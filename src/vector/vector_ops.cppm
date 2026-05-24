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
	constexpr auto operator<=>(const Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
		using comparison_t = decltype(a[0] <=> b[0]);

		for (size_t i = 0; i < N; ++i)
		{
			if (auto cmp = a[i] <=> b[i]; cmp != 0)
				return cmp;
		}

		return comparison_t::equivalent;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr bool operator==(const Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
		for (size_t i = 0; i < N; ++i)
			if (a[i] != b[i])
				return false;
		return true;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr bool operator!=(const Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
		return !(a == b);
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator+(const Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && supports_add<T, N>)
		{
			using ops = ops<T, N>;

			Vec<T, N> r{};
			auto va = ops::load(a.v);
			auto vb = ops::load(b.v);
			ops::store(r.v, va + vb);

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
		if constexpr (std::is_same_v<T, U> && supports_sub<T, N>)
		{
			using ops = ops<T, N>;

			Vec<T, N> r{};
			auto va = ops::load(a.v);
			auto vb = ops::load(b.v);
			ops::store(r.v, va - vb);

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
		if constexpr (std::is_same_v<T, U> && supports_mul<T, N>)
		{
			using ops = ops<T, N>;

			Vec<T, N> r{};
			auto va = ops::load(a.v);
			auto vb = ops::load(b.v);
			ops::store(r.v, va * vb);

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
		if constexpr (std::is_same_v<T, U> && supports_div<T, N>)
		{
			using ops = ops<T, N>;

			Vec<T, N> r{};
			auto va = ops::load(a.v);
			auto vb = ops::load(b.v);
			ops::store(r.v, va / vb);

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
		if constexpr (std::is_same_v<T, U> && supports_mul<T, N> && supports_splat<T, N>)
		{
			using ops = ops<T, N>;

			Vec<T, N> r{};
			auto va = ops::load(a.v);
			auto vb = ops::splat(s);
			ops::store(r.v, va + vb);

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
		if constexpr (std::is_same_v<T, U> && supports_add<T, N> && supports_splat<T, N>)
		{
			using ops = ops<T, N>;

			Vec<T, N> r{};
			auto vs = ops::splat(s);
			auto vb = ops::load(b.v);
			ops::store(r.v, vs - vb);

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
		if constexpr (supports_sub<T, N> && supports_splat<T, N>)
		{
			using ops = ops<T, N>;

			Vec<std::common_type_t<T, U>, N> r;

			auto va = ops::load(a.v);
			auto vs = ops::splat(s);
			ops::store(r.v, va - vs);

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
		if constexpr (supports_sub<T, N> && supports_splat<T, N>)
		{
			using ops = ops<T, N>;

			Vec<std::common_type_t<T, U>, N> r;

			auto vs = ops::load(s);
			auto vb = ops::load(b.v);
			ops::store(r.v, vs - vb);

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
		if constexpr (supports_mul<T, N> && supports_splat<T, N>)
		{
			using ops = ops<T, N>;

			Vec<std::common_type_t<T, U>, N> r;

			auto va = ops::load(a.v);
			auto vs = ops::splat(s);
			ops::store(r.v, va * vs);

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
		if constexpr (supports_mul<T, N> && supports_splat<T, N>)
		{
			using ops = ops<T, N>;

			Vec<std::common_type_t<T, U>, N> r;

			auto vs = ops::splat(s);
			auto vb = ops::load(b.v);
			ops::store(r.v, vs * vb);

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
		if constexpr (supports_div<T, N> && supports_splat<T, N>)
		{
			using ops = ops<T, N>;

			Vec<std::common_type_t<T, U>, N> r;

			auto va = ops::load(a);
			auto vs = ops::splat(s);
			ops::store(r.v, va / vs);

			return r;
		}
#endif
		Vec<std::common_type_t<T, U>, N> ret;
		for (size_t i = 0; i < N; ++i)
			ret[i] = a[i] / s;
		return ret;
	}
} // namespace mag
