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
#include <concepts>
#include <type_traits>
#include <valarray>
export module mag:vector_ops;

import :concepts;
import :vector;

#ifdef MAG_ENABLE_SIMD
import :simd_ops;
#endif

export namespace mag
{
	template <Numeric T, Numeric U, size_t N>
		requires std::convertible_to<U, T>
	constexpr Vec<T, N>& operator+=(Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && simd::supports_add<T, N>)
		{
			using ops = simd::ops<T, N>;

			auto va = ops::load(a.v);
			auto vb = ops::load(b.v);
			auto vr = ops::add(va, vb);
			ops::store(a.v, vr);
			return a;
		}
#endif
		for (size_t i = 0; i < N; ++i)
		{
			a[i] += b[i];
		}
		return a;
	}

	template <Numeric T, Numeric U, size_t N>
		requires std::convertible_to<U, T>
	constexpr Vec<T, N>& operator-=(Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && simd::supports_subtraction<T, N>)
		{
			using ops = simd::ops<T, N>;

			auto va = ops::load(a.v);
			auto vb = ops::load(b.v);
			auto vr = ops::sub(va, vb);
			ops::store(a.v, vr);
			return a;
		}
#endif
		for (size_t i = 0; i < N; ++i)
		{
			a[i] -= b[i];
		}
		return a;
	}

	template <Numeric T, Numeric U, size_t N>
		requires std::convertible_to<U, T>
	constexpr Vec<T, N>& operator*=(Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && simd::supports_multiplication<T, N>)
		{
			using ops = simd::ops<T, N>;

			auto va = ops::load(a.v);
			auto vb = ops::load(b.v);
			auto vr = ops::mul(va, vb);
			ops::store(a.v, vr);
			return a;
		}
#endif
		for (size_t i = 0; i < N; ++i)
		{
			a[i] *= b[i];
		}
		return a;
	}

	template <Numeric T, Numeric U, size_t N>
		requires std::convertible_to<U, T>
	constexpr Vec<T, N>& operator/=(Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && simd::supports_division<T, N>)
		{
			using ops = simd::ops<T, N>;

			auto va = ops::load(a.v);
			auto vb = ops::load(b.v);
			auto vr = ops::div(va, vb);
			ops::store(a.v, vr);
			return a;
		}
#endif
		for (size_t i = 0; i < N; ++i)
		{
			a[i] /= b[i];
		}
		return a;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator+=(Vec<T, N>& a, const U& s) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (simd::supports_add<T, N>)
		{
			using ops = simd::ops<T, N>;

			auto va = ops::load(a.v);
			auto vs = ops::splat(s);
			auto vr = ops::add(va, vs);
			ops::store(a.v, vr);
			return a;
		}
#endif
		for (size_t i = 0; i < N; ++i)
			a[i] = a[i] + s;
		return a;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator-=(Vec<T, N>& a, const U& s) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (simd::supports_subtraction<T, N>)
		{
			using ops = simd::ops<T, N>;

			auto va = ops::load(a.v);
			auto vs = ops::splat(s);
			auto vr = ops::sub(va, vs);
			ops::store(a.v, vr);
			return a;
		}
#endif
		for (size_t i = 0; i < N; ++i)
			a[i] = a[i] - s;
		return a;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator*=(Vec<T, N>& a, const U& s) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (simd::supports_multiplication<T, N>)
		{
			using ops = simd::ops<T, N>;

			auto va = ops::load(a.v);
			auto vs = ops::splat(s);
			auto vr = ops::mul(va, vs);
			ops::store(a.v, vr);
			return a;
		}
#endif
		for (size_t i = 0; i < N; ++i)
			a[i] = a[i] * s;
		return a;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator/=(Vec<T, N>& a, const U& s) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (simd::supports_division<T, N>)
		{
			using ops = simd::ops<T, N>;

			auto va = ops::load(a.v);
			auto vs = ops::splat(s);
			auto vr = ops::div(va, vs);
			ops::store(a.v, vr);
			return a;
		}
#endif
		for (size_t i = 0; i < N; ++i)
			a[i] = a[i] / s;
		return a;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator+(const Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && simd::supports_add<T, N>)
		{
			using ops = simd::ops<T, N>;

			Vec<T, N> r{};
			auto va = ops::load(a.v);
			auto vb = ops::load(b.v);
			auto vr = ops::add(va, vb);
			ops::store(r.v, vr);
			return r;
		}
#endif
		using R = std::common_type_t<T, U>;

		Vec<R, N> ret;
		for (size_t i = 0; i < N; ++i)
			ret[i] = a[i] + b[i];
		return ret;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator-(const Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && simd::supports_subtraction<T, N>)
		{
			using ops = simd::ops<T, N>;

			Vec<T, N> r{};
			auto va = ops::load(a.v);
			auto vb = ops::load(b.v);
			auto vr = ops::sub(va, vb);
			ops::store(r.v, vr);
			return r;
		}
#endif
		Vec<std::common_type_t<T, U>, N> ret;
		for (size_t i = 0; i < N; ++i)
			ret[i] = a[i] - b[i];
		return ret;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator*(const Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && simd::supports_multiplication<T, N>)
		{
			using ops = simd::ops<T, N>;

			Vec<T, N> r{};
			auto va = ops::load(a.v);
			auto vb = ops::load(b.v);
			auto vr = ops::mul(va, vb);
			ops::store(r.v, vr);
			return r;
		}
#endif
		Vec<std::common_type_t<T, U>, N> ret;
		for (size_t i = 0; i < N; ++i)
			ret[i] = a[i] * b[i];
		return ret;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator/(const Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && simd::supports_division<T, N>)
		{
			using ops = simd::ops<T, N>;

			Vec<T, N> r{};
			auto va = ops::load(a.v);
			auto vb = ops::load(b.v);
			auto vr = ops::div(va, vb);
			ops::store(r.v, vr);
			return r;
		}
#endif
		Vec<std::common_type_t<T, U>, N> ret;
		for (size_t i = 0; i < N; ++i)
			ret[i] = a[i] / b[i];
		return ret;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator+(const Vec<T, N>& a, U s) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && simd::supports_multiplication<T, N> &&
					  simd::supports_splat<T, N>)
		{
			using ops = simd::ops<T, N>;

			Vec<T, N> r{};
			auto va = ops::load(a.v);
			auto vb = ops::splat(s);
			auto vr = ops::add(va, vb);
			ops::store(r.v, vr);
			return r;
		}
#endif
		Vec<std::common_type_t<T, U>, N> ret;
		for (size_t i = 0; i < N; ++i)
			ret[i] = a[i] + s;
		return ret;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator+(U s, const Vec<T, N>& b) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && simd::supports_add<T, N> &&
					  simd::supports_splat<T, N>)
		{
			using ops = simd::ops<T, N>;

			Vec<T, N> r{};
			auto vs = ops::splat(s);
			auto vb = ops::load(b.v);
			auto vr = ops::add(vs, vb);
			ops::store(r.v, vr);
			return r;
		}
#endif
		Vec<std::common_type_t<T, U>, N> ret;
		for (size_t i = 0; i < N; ++i)
			ret[i] = b[i] + s;
		return ret;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator-(const Vec<T, N>& a, U s) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (simd::supports_subtraction<T, N> && simd::supports_splat<T, N>)
		{
			using ops = simd::ops<T, N>;

			Vec<std::common_type_t<T, U>, N> r;

			auto va = ops::load(a.v);
			auto vs = ops::splat(s);
			auto vr = ops::sub(va, vs);
			ops::store(r.v, vr);
			return r;
		}
#endif
		Vec<std::common_type_t<T, U>, N> ret;
		for (size_t i = 0; i < N; ++i)
			ret[i] = a[i] - s;
		return ret;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator-(U s, const Vec<T, N>& b) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (simd::supports_subtraction<T, N> && simd::supports_splat<T, N>)
		{
			using ops = simd::ops<T, N>;

			Vec<std::common_type_t<T, U>, N> r;

			auto vs = ops::load(s);
			auto vb = ops::load(b.v);
			auto vr = ops::sub(vs, vb);
			ops::store(r.v, vr);
			return r;
		}
#endif
		Vec<std::common_type_t<T, U>, N> ret;
		for (size_t i = 0; i < N; ++i)
			ret[i] = s - b[i];
		return ret;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator*(const Vec<T, N>& a, U s) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (simd::supports_multiplication<T, N> && simd::supports_splat<T, N>)
		{
			using ops = simd::ops<T, N>;

			Vec<std::common_type_t<T, U>, N> r;

			auto va = ops::load(a.v);
			auto vs = ops::splat(s);
			auto vr = ops::mul(va, vs);
			ops::store(r.v, vr);
			return r;
		}
#endif
		Vec<std::common_type_t<T, U>, N> ret;
		for (size_t i = 0; i < N; ++i)
			ret[i] = a[i] * s;
		return ret;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator*(U s, const Vec<T, N>& b) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (simd::supports_multiplication<T, N> && simd::supports_splat<T, N>)
		{
			using ops = simd::ops<T, N>;

			Vec<std::common_type_t<T, U>, N> r;

			auto vs = ops::splat(s);
			auto vb = ops::load(b.v);
			auto vr = ops::mul(vs, vb);
			ops::store(r.v, vr);
			return r;
		}
#endif
		Vec<std::common_type_t<T, U>, N> ret;
		for (size_t i = 0; i < N; ++i)
			ret[i] = b[i] * s;
		return ret;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator/(const Vec<T, N>& a, U s) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (simd::supports_division<T, N> && simd::supports_splat<T, N>)
		{
			using ops = simd::ops<T, N>;

			Vec<std::common_type_t<T, U>, N> r;

			auto va = ops::load(a.v);
			auto vs = ops::splat(s);
			auto vr = ops::div(va, vs);
			ops::store(r.v, vr);
			return r;
		}
#endif
		Vec<std::common_type_t<T, U>, N> ret;
		for (size_t i = 0; i < N; ++i)
			ret[i] = a[i] / s;
		return ret;
	}

	template <Numeric T, Numeric U, size_t N>
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

	template <typename Derived, Numeric T, size_t N>
	template <Numeric U>
	constexpr auto IVec<Derived, T, N>::dot(const Vec<U, N>& o) const noexcept
	{
		return mag::dot(derived(), o);
	}

	template <Numeric T, size_t N>
	constexpr T distance(const Vec<T, N>& a, const Vec<T, N>& b) noexcept
	{
		return (a - b).length();
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto lerp(const Vec<T, N>& a, const Vec<U, N>& b, T t) noexcept
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

	template <Numeric T, size_t N>
	constexpr Vec<T, N> normalize(Vec<T, N> v)
	{
		float length = std::sqrt(v.x * v.x + v.y * v.y);
		return (length > 0.0f) ? Vec<T, N>{v.x / length, v.y / length} : Vec<T, N>{0.0f, 0.0f};
	}
} // namespace mag
