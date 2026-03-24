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
#include <cstddef>
#include <type_traits>
export module mag:vector_ops;

import :concepts;
import :vector;
#if defined(MAG_ENABLE_SIMD)
#ifdef __ARM_NEON
import :neon_ops;
import :neon_traits;
#endif
import :simd_ops;
import :simd_traits;
#endif

namespace mag
{
	export template <Numeric T, size_t N, Numeric U>
	constexpr auto operator+=(Vec<T, N>& v, const U& o) noexcept
	{
		for (size_t i = 0; i < N; ++i)
			v[i] = v[i] + o;
		return v;
	}

	export template <Numeric T, Numeric U, size_t N>
	constexpr auto operator+(const Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && simd_traits<T, N>::supported &&
					  simd_traits<T, N>::supports_add)
		{
			using ops = simd_ops<T, N>;

			Vec<T, N> r{};
			auto va = ops::load(a.v);
			auto vb = ops::load(b.v);
			auto vr = ops::op(va, vb, add_op{});
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

	export template <Numeric T, Numeric U, size_t N>
	constexpr auto operator-(const Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && simd_traits<T, N>::supported &&
					  simd_traits<T, N>::supports_sub)
		{
			using ops = simd_ops<T, N>;

			Vec<T, N> r{};
			auto va = ops::load(a.v);
			auto vb = ops::load(b.v);
			auto vr = ops::op(va, vb, sub_op{});
			ops::store(r.v, vr);
			return r;
		}
#endif
		Vec<std::common_type_t<T, U>, N> ret;
		for (size_t i = 0; i < N; ++i)
			ret[i] = a[i] - b[i];
		return ret;
	}

	export template <Numeric U, Numeric T, size_t N>
	constexpr auto operator*(U t, const Vec<T, N>& v) noexcept
	{
		return v * t;
	}

	export template <Numeric T, Numeric U, size_t N>
	constexpr auto operator*(const Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && simd_traits<T, N>::supported &&
					  simd_traits<T, N>::supports_mul)
		{
			using ops = simd_ops<T, N>;

			Vec<T, N> r{};
			auto va = ops::load(a.v);
			auto vb = ops::load(b.v);
			auto vr = ops::op(va, vb, mul_op{});
			ops::store(r.v, vr);
			return r;
		}
#endif
		Vec<std::common_type_t<T, U>, N> ret;
		for (size_t i = 0; i < N; ++i)
			ret[i] = a[i] * b[i];
		return ret;
	}

	export template <Numeric T, Numeric U, size_t N>
	constexpr auto operator/(const Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && simd_traits<T, N>::supported &&
					  simd_traits<T, N>::supports_div)
		{
			using ops = simd_ops<T, N>;

			Vec<T, N> r{};
			auto va = ops::load(a.v);
			auto vb = ops::load(b.v);
			auto vr = ops::op(va, vb, div_op{});
			ops::store(r.v, vr);
			return r;
		}
#endif
		Vec<std::common_type_t<T, U>, N> ret;
		for (size_t i = 0; i < N; ++i)
			ret[i] = a[i] / b[i];
		return ret;
	}

	export template <Numeric T, Numeric U, size_t N>
	constexpr auto operator+(const Vec<T, N>& v, U s) noexcept
	{
		Vec<std::common_type_t<T, U>, N> ret;
		for (size_t i = 0; i < N; ++i)
			ret[i] = v[i] + s;
		return ret;
	}

	export template <Numeric T, Numeric U, size_t N>
	constexpr auto operator+(U s, const Vec<T, N>& v) noexcept
	{
		return v + s;
	}

	export template <Numeric T, Numeric U, size_t N>
	constexpr auto operator-(const Vec<T, N>& v, U s) noexcept
	{
		Vec<std::common_type_t<T, U>, N> ret;
		for (size_t i = 0; i < N; ++i)
			ret[i] = v[i] - s;
		return ret;
	}

	export template <Numeric T, Numeric U, size_t N>
	constexpr auto operator-(U s, const Vec<T, N>& v) noexcept
	{
		Vec<std::common_type_t<T, U>, N> ret;
		for (size_t i = 0; i < N; ++i)
			ret[i] = s - v[i];
		return ret;
	}

	export template <Numeric T, Numeric U, size_t N>
	constexpr auto operator*(const Vec<T, N>& v, U s) noexcept
	{
		Vec<std::common_type_t<T, U>, N> ret;
		for (size_t i = 0; i < N; ++i)
			ret[i] = v[i] * s;
		return ret;
	}

	export template <Numeric T, Numeric U, size_t N>
	constexpr auto operator/(const Vec<T, N>& v, U s) noexcept
	{
		Vec<std::common_type_t<T, U>, N> ret;
		for (size_t i = 0; i < N; ++i)
			ret[i] = v[i] / s;
		return ret;
	}

	export template <Numeric T, Numeric U, size_t N>
	constexpr auto dot(const Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
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
	constexpr auto lerp(const Vec<T, N>& a, const Vec<U, N>& b, T t) noexcept
	{
		return a + t * (b - a);
	}
} // namespace mag
