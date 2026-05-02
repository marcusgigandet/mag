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

		constexpr std::reverse_iterator<T*> rbegin() noexcept
		{
			return std::reverse_iterator<T*>(end());
		}
		constexpr std::reverse_iterator<const T*> rbegin() const noexcept
		{
			return std::reverse_iterator<const T*>(end());
		}
		constexpr std::reverse_iterator<T*> rend() noexcept
		{
			return std::reverse_iterator<T*>(begin());
		}
		constexpr std::reverse_iterator<const T*> rend() const noexcept
		{
			return std::reverse_iterator<const T*>(begin());
		}

		/**************************/
		/* Const iterator support */
		/**************************/

		constexpr const T* cbegin() const noexcept { return begin(); }
		constexpr const T* cend() const noexcept { return end(); }
		constexpr std::reverse_iterator<const T*> crbegin() const noexcept { return rbegin(); }
		constexpr std::reverse_iterator<const T*> crend() const noexcept { return rend(); }

		constexpr T& operator[](size_t i) noexcept { return derived().v[i]; }
		constexpr const T& operator[](size_t i) const noexcept { return derived().v[i]; }

		template <Numeric U>
		constexpr auto operator<=>(const Vec<U, N>& o) const noexcept
		{
			for (size_t i = 0; i < N; ++i)
				if (auto cmp = (*this)[i] <=> o[i]; cmp != 0)
					return cmp;
			return std::strong_ordering::equal;
		}

		template <Numeric U>
		constexpr bool operator==(const Vec<U, N>& o) const noexcept
		{
			for (size_t i = 0; i < N; ++i)
				if (derived()[i] != o[i])
					return false;
			return true;
		}

		template <Numeric U>
		constexpr bool operator!=(const Vec<U, N>& o) const noexcept
		{
			return !(*this == o);
		}

		constexpr T length() const noexcept
		{
			T ret = 0;
			for (size_t i = 0; i < N; ++i)
			{
				ret += derived()[i] * derived()[i];
			}

			return std::sqrt(ret);
		}

		constexpr Derived normalized() const noexcept
		{
			Derived ret = derived();
			T len = ret.length();
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
		constexpr auto dot(const Vec<U, N>& o) const noexcept;

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
