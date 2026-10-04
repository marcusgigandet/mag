/*
 * SPDX-FileCopyrightText: 2026 Marcus Gigandet
 *
 * SPDX-License-Identifier: Apache-2.0
 */

module;
#include "typedefs.hpp"
#include <cstddef>
#include <type_traits>
export module mag:vector_ops;

import :vector;

#ifdef MAG_ENABLE_SIMD
import mag.simd;
using namespace MAG_NAMESPACE::simd;
#endif

export namespace MAG_NAMESPACE
{
	template <Numeric T, Numeric U, std::size_t N>
	constexpr auto operator<=>(const Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
		using comparison_t = decltype(a[0] <=> b[0]);

		for (std::size_t i = 0; i < N; ++i)
		{
			if (auto cmp = a[i] <=> b[i]; cmp != 0)
			{
				return cmp;
			}
		}

		return comparison_t::equivalent;
	}

	template <Numeric T, Numeric U, std::size_t N>
	constexpr bool operator==(const Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
		for (std::size_t i = 0; i < N; ++i)
		{
			if (a[i] != b[i])
			{
				return false;
			}
		}
		return true;
	}

	template <Numeric T, Numeric U, std::size_t N>
	constexpr bool operator!=(const Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
		return !(a == b);
	}

	template <Numeric T, Numeric U, std::size_t N>
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

		for (std::size_t i = 0; i < N; ++i)
		{
			ret[i] = a[i] + b[i];
		}
		return ret;
	}

	template <Numeric T, Numeric U, std::size_t N>
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

		for (std::size_t i = 0; i < N; ++i)
		{
			ret[i] = a[i] - b[i];
		}
		return ret;
	}

	template <Numeric T, Numeric U, std::size_t N>
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

		for (std::size_t i = 0; i < N; ++i)
		{
			ret[i] = a[i] * b[i];
		}
		return ret;
	}

	template <Numeric T, Numeric U, std::size_t N>
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

		for (std::size_t i = 0; i < N; ++i)
		{
			ret[i] = a[i] / b[i];
		}
		return ret;
	}

	template <Numeric T, Numeric U, std::size_t N>
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

		for (std::size_t i = 0; i < N; ++i)
		{
			ret[i] = a[i] + s;
		}
		return ret;
	}

	template <Numeric T, Numeric U, std::size_t N>
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

		for (std::size_t i = 0; i < N; ++i)
		{
			ret[i] = b[i] + s;
		}
		return ret;
	}

	template <Numeric T, Numeric U, std::size_t N>
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

		for (std::size_t i = 0; i < N; ++i)
		{
			ret[i] = a[i] - s;
		}
		return ret;
	}

	template <Numeric T, Numeric U, std::size_t N>
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

		for (std::size_t i = 0; i < N; ++i)
		{
			ret[i] = s - b[i];
		}
		return ret;
	}

	template <Numeric T, Numeric U, std::size_t N>
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

		for (std::size_t i = 0; i < N; ++i)
		{
			ret[i] = a[i] * s;
		}
		return ret;
	}

	template <Numeric T, Numeric U, std::size_t N>
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

		for (std::size_t i = 0; i < N; ++i)
		{
			ret[i] = b[i] * s;
		}
		return ret;
	}

	template <Numeric T, Numeric U, std::size_t N>
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

		for (std::size_t i = 0; i < N; ++i)
		{
			ret[i] = a[i] / s;
		}
		return ret;
	}
} // namespace MAG_NAMESPACE
