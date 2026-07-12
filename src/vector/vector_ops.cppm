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
#include "typedefs.hpp"
#include <cstddef>
#include <type_traits>
export module mag:vector_ops;

import :vector;

#ifdef MAG_ENABLE_SIMD
import mag.simd;
using namespace mag::simd;
#endif

export namespace MAG_NAMESPACE
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
		using R = std::common_type_t<T, U>;
		Vec<R, N> ret;

#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && supports_add<T, N>)
		{
			Simd<T, N> va{a.v};
			Simd<T, N> vb{b.v};

			auto vr{va + vb};
			vr.store(ret.v);

			return ret;
		}
#endif

		for (size_t i = 0; i < N; ++i)
			ret[i] = a[i] + b[i];
		return ret;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator-(const Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
		using R = std::common_type_t<T, U>;
		Vec<R, N> ret;

#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && supports_sub<T, N>)
		{
			Simd<T, N> va{a.v};
			Simd<T, N> vb{b.v};

			auto vr{va - vb};
			vr.store(ret.v);

			return ret;
		}
#endif

		for (size_t i = 0; i < N; ++i)
			ret[i] = a[i] - b[i];
		return ret;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator*(const Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
		using R = std::common_type_t<T, U>;
		Vec<R, N> ret;

#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && supports_mul<T, N>)
		{
			Simd<T, N> va{a.v};
			Simd<T, N> vb{b.v};

			auto vr{va * vb};
			vr.store(ret.v);

			return ret;
		}
#endif

		for (size_t i = 0; i < N; ++i)
			ret[i] = a[i] * b[i];
		return ret;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator/(const Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
		using R = std::common_type_t<T, U>;
		Vec<R, N> ret;

#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && supports_div<T, N>)
		{
			Simd<T, N> va{a.v};
			Simd<T, N> vb{b.v};

			auto vr{va / vb};
			vr.store(ret.v);

			return ret;
		}
#endif

		for (size_t i = 0; i < N; ++i)
			ret[i] = a[i] / b[i];
		return ret;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator+(const Vec<T, N>& a, U s) noexcept
	{
		using R = std::common_type_t<T, U>;
		Vec<R, N> ret;

#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && supports_mul<T, N> && supports_splat<T, N>)
		{
			Simd<T, N> va{a.v};
			Simd<T, N> vb{s};

			auto vr{va + vb};
			vr.store(ret.v);

			return ret;
		}
#endif

		for (size_t i = 0; i < N; ++i)
			ret[i] = a[i] + s;
		return ret;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator+(U s, const Vec<T, N>& b) noexcept
	{
		using R = std::common_type_t<T, U>;
		Vec<R, N> ret;

#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && supports_add<T, N> && supports_splat<T, N>)
		{
			Simd<T, N> va{s};
			Simd<T, N> vb{b.v};

			auto vr{va + vb};
			vr.store(ret.v);

			return ret;
		}
#endif

		for (size_t i = 0; i < N; ++i)
			ret[i] = b[i] + s;
		return ret;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator-(const Vec<T, N>& a, U s) noexcept
	{
		using R = std::common_type_t<T, U>;
		Vec<R, N> ret;

#ifdef MAG_ENABLE_SIMD
		if constexpr (supports_sub<T, N> && supports_splat<T, N>)
		{
			Simd<T, N> va{a.v};
			Simd<T, N> vb{s};

			auto vr{va - vb};
			vr.store(ret.v);

			return ret;
		}
#endif

		for (size_t i = 0; i < N; ++i)
			ret[i] = a[i] - s;
		return ret;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator-(U s, const Vec<T, N>& b) noexcept
	{
		using R = std::common_type_t<T, U>;
		Vec<R, N> ret;

#ifdef MAG_ENABLE_SIMD
		if constexpr (supports_sub<T, N> && supports_splat<T, N>)
		{
			Simd<T, N> va{s};
			Simd<T, N> vb{b.v};

			auto vr{va - vb};
			vr.store(ret.v);

			return ret;
		}
#endif

		for (size_t i = 0; i < N; ++i)
			ret[i] = s - b[i];
		return ret;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator*(const Vec<T, N>& a, U s) noexcept
	{
		using R = std::common_type_t<T, U>;
		Vec<R, N> ret;

#ifdef MAG_ENABLE_SIMD
		if constexpr (supports_mul<T, N> && supports_splat<T, N>)
		{
			Simd<T, N> va{a.v};
			Simd<T, N> vb{s};

			auto vr{va * vb};
			vr.store(ret.v);

			return ret;
		}
#endif

		for (size_t i = 0; i < N; ++i)
			ret[i] = a[i] * s;
		return ret;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator*(U s, const Vec<T, N>& b) noexcept
	{
		using R = std::common_type_t<T, U>;
		Vec<R, N> ret;

#ifdef MAG_ENABLE_SIMD
		if constexpr (supports_mul<T, N> && supports_splat<T, N>)
		{
			Simd<T, N> va{s};
			Simd<T, N> vb{b.v};

			auto vr{va * vb};
			vr.store(ret.v);

			return ret;
		}
#endif

		for (size_t i = 0; i < N; ++i)
			ret[i] = b[i] * s;
		return ret;
	}

	template <Numeric T, Numeric U, size_t N>
	constexpr auto operator/(const Vec<T, N>& a, U s) noexcept
	{
		using R = std::common_type_t<T, U>;
		Vec<R, N> ret;

#ifdef MAG_ENABLE_SIMD
		if constexpr (supports_div<T, N> && supports_splat<T, N>)
		{
			Simd<T, N> va{a.v};
			Simd<T, N> vb{s};

			auto vr{va / vb};
			vr.store(ret.v);

			return ret;
		}
#endif

		for (size_t i = 0; i < N; ++i)
			ret[i] = a[i] / s;
		return ret;
	}
} // namespace MAG_NAMESPACE
