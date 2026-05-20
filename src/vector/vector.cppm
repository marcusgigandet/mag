/*
 * Copyright 2026 Marcus Gigandet
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

module;
#include <algorithm>
#include <cmath>
#include <sstream>
#include <string>
export module mag:vector;

import :concepts;

#ifdef MAG_ENABLE_SIMD
import :simd_ops;
#endif

namespace mag
{
	/**
	 * @brief Generic N-dimensional vector for numerical computations.
	 *
	 * @tparam T Numeric type of the vector elements.
	 * @tparam N Dimension of the vector.
	 */
	export template <Numeric T, size_t N>
	struct Vec;


	export template <Numeric T, size_t N>
	constexpr Vec<T, N> normalize(Vec<T, N> v)
	{
		float length{std::sqrt(v.x * v.x + v.y * v.y)};
		return (length > 0.0f) ? Vec<T, N>{v.x / length, v.y / length} : Vec<T, N>{0.0f, 0.0f};
	}

	export template <Numeric T, Numeric U, size_t N>
	constexpr auto dot(const Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && simd::supports_multiplication<T, N>)
		{
			using ops = simd::ops<T, N>;

			auto va = ops::load(a.v);
			auto vb = ops::load(b.v);
			auto vm = ops::mul(va, vb);

			return static_cast<T>(ops::horizontal_sum(vm));
		}
#endif
		using R = std::common_type_t<T, U>;
		R ret = 0;
		for (size_t i = 0; i < N; ++i)
			ret += a[i] * b[i];
		return ret;
	}

	export template <Numeric T, size_t N>
	constexpr T distance(const Vec<T, N>& a, const Vec<T, N>& b) noexcept
	{
		return (a - b).length();
	}

	export template <Numeric T, Numeric U, size_t N>
	constexpr Vec<T, N> lerp(const Vec<T, N>& a, const Vec<U, N>& b, T t) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && simd::supports_splat<T, N> &&
					  simd::supports_add<T, N> && simd::supports_subtraction<T, N> &&
					  simd::supports_multiplication<T, N>)
		{
			using ops = simd::ops<T, N>;

			Vec<T, N> r;
			auto va = ops::load(a.v);
			auto vb = ops::load(b.v);
			auto vt = ops::splat(static_cast<T>(t));
			auto vr = ops::add(va, ops::mul(ops::sub(vb, va), vt));
			ops::store(r.v, vr);
			return r;
		}
#endif
		return a + t * (b - a);
	}


	/**
	 * @brief Base class for N-dimensional vectors using the Curiously Recurring Template Pattern
	 * (CRTP). Provides common vector operations for derived vector classes.
	 *
	 * @tparam Derived The derived vector class (e.g., `Vec<T, N>`).
	 * @tparam T Numeric type of the vector elements.
	 * @tparam N Dimension of the vector.
	 */
	template <typename Derived, Numeric T, size_t N>
	struct IVec
	{
	private:
		/**
		 * @brief Returns a reference to the derived vector type.
		 *
		 * Provides access to the concrete vector implementation using CRTP.
		 * This allows the base class to call functions or access data defined
		 * in the derived class without virtual dispatch.
		 *
		 * @return Reference to the derived vector instance.
		 */
		constexpr Derived& derived() noexcept { return static_cast<Derived&>(*this); }

		/**
		 * @brief Returns a const reference to the derived vector type.
		 *
		 * Const-qualified version of derived(), enabling read-only access
		 * to the concrete vector implementation from const member functions.
		 *
		 * @return Const reference to the derived vector instance.
		 */
		constexpr const Derived& derived() const noexcept
		{
			return static_cast<const Derived&>(*this);
		}

	public:
		/********************/
		/* Iterator support */
		/********************/

		constexpr T* begin() noexcept { return &derived()[0]; }
		constexpr const T* begin() const noexcept { return &derived()[0]; }
		constexpr T* end() noexcept { return &derived()[0] + N; }
		constexpr const T* end() const noexcept { return &derived()[0] + N; }

		/****************************/
		/* Reverse iterator support */
		/****************************/

		// clang-format off
		constexpr std::reverse_iterator<T*> rbegin() noexcept { return std::reverse_iterator<T*>(end()); }
		constexpr std::reverse_iterator<const T*> rbegin() const noexcept { return std::reverse_iterator<const T*>(end()); }
		constexpr std::reverse_iterator<T*> rend() noexcept { return std::reverse_iterator<T*>(begin()); }
		constexpr std::reverse_iterator<const T*> rend() const noexcept { return std::reverse_iterator<const T*>(begin()); }
		// clang-format on

		/**************************/
		/* Const iterator support */
		/**************************/

		constexpr const T* cbegin() const noexcept { return begin(); }
		constexpr const T* cend() const noexcept { return end(); }
		constexpr std::reverse_iterator<const T*> crbegin() const noexcept { return rbegin(); }
		constexpr std::reverse_iterator<const T*> crend() const noexcept { return rend(); }

		constexpr T& operator[](size_t i) noexcept { return derived().v[i]; }
		constexpr const T& operator[](size_t i) const noexcept { return derived().v[i]; }

		constexpr T length() const noexcept
		{
#ifdef MAG_ENABLE_SIMD
			if constexpr (simd::supports_multiplication<T, N> &&
						  simd::supports_horizontal_sum<T, N>)
			{
				using ops = simd::ops<T, N>;

				auto v = ops::load(&derived()[0]);
				auto sq = ops::mul(v, v);

				return std::sqrt(ops::horizontal_sum(sq));
			}
#endif
			T ret{0};
			for (size_t i = 0; i < N; ++i)
				ret += derived()[i] * derived()[i];

			return std::sqrt(ret);
		}

		constexpr Derived normalized() const noexcept
		{
			Derived ret = derived();
			T len{ret.length()};
			if (len > 0)
				ret /= len;
			return ret;
		}

		constexpr Derived clamped(T min, T max) const noexcept
		{
			Derived ret = derived();
			for (size_t i = 0; i < N; ++i)
				ret[i] = std::clamp(derived()[i], min, max);
			return ret;
		}

		template <Numeric U>
		constexpr auto dot(const Vec<U, N>& o) const noexcept
		{
			return mag::dot(derived(), o);
		}

		[[nodiscard]] std::string toString() const noexcept
		{
			std::ostringstream oss;
			oss << "Vec" << N << "(";
			for (size_t i = 0; i < N; ++i)
			{
				if (i != N - 1)
					oss << derived()[i] << ", ";
				else
					oss << derived()[i];
			}
			oss << ")";
			return oss.str();
		}
	};

	template <Numeric T, size_t N>
	struct Vec : IVec<Vec<T, N>, T, N>
	{
		T v[N]{};

		constexpr Vec() = default;

		explicit constexpr Vec(T val)
		{
			for (size_t i = 0; i < N; ++i)
				v[i] = val;
		}
	};
} // namespace mag
