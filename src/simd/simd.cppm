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
#include <span>
export module mag.simd:simd;

import :abi;
import :concepts;
import :ops;
import mag.core;

namespace mag::simd
{
	/**
	 * @brief SIMD interface.
	 *
	 * Provides the primitive SIMD operations for a given type and lane count.
	 *
	 * @tparam T Scalar element type.
	 * @tparam N SIMD lane count.
	 */
	export template <Numeric T, size_t N, simd_isa Isa = default_isa>
	class Simd
	{
		using native_type = ops_impl<T, N, Isa>::native_t;

		/// Underlying SIMD register storage
		native_type m_native{};

	public:
		using value_type = T;
		static constexpr size_t lanes = N;

		[[nodiscard]] static consteval size_t size() noexcept { return N; }

		MAG_INLINE Simd() noexcept = default;

		template <std::convertible_to<T> U>
		MAG_INLINE explicit Simd(U s) noexcept :
			m_native(ops_impl<T, N, default_isa>::splat(static_cast<T>(s)))
		{
		}

		MAG_INLINE explicit Simd(const std::span<const T, N> data) noexcept :
			m_native(ops_impl<T, N, default_isa>::load(data.data()))
		{
		}

		MAG_INLINE explicit Simd(const T* ptr) noexcept : m_native(ops_impl<T, N, Isa>::load(ptr))
		{
		}

		template <typename... Args>
			requires(sizeof...(Args) == N) && (std::convertible_to<Args, T> && ...)
		MAG_INLINE explicit Simd(Args... args) noexcept
		{
			alignas(sizeof(native_type)) T tmp[N]{static_cast<T>(args)...};
			m_native = ops_impl<T, N, default_isa>::load(tmp);
		}

		MAG_INLINE explicit Simd(native_type data) noexcept : m_native(data) {}

		Simd(const Simd&) noexcept = default;
		Simd(Simd&&) noexcept = default;
		Simd& operator=(const Simd&) noexcept = default;
		Simd& operator=(Simd&&) noexcept = default;

		MAG_INLINE friend Simd operator+(const Simd& a, const Simd& b)
			requires supports_add<T, N, default_isa>
		{
			return Simd{ops_impl<T, N, default_isa>::add(a.m_native, b.m_native)};
		}
		MAG_INLINE friend Simd operator-(const Simd& a, const Simd& b)
			requires supports_sub<T, N, default_isa>
		{
			return Simd{ops_impl<T, N, default_isa>::sub(a.m_native, b.m_native)};
		}
		MAG_INLINE friend Simd operator*(const Simd& a, const Simd& b)
			requires supports_mul<T, N, default_isa>
		{
			return Simd{ops_impl<T, N, default_isa>::mul(a.m_native, b.m_native)};
		}
		MAG_INLINE friend Simd operator/(const Simd& a, const Simd& b)
			requires supports_div<T, N, default_isa>
		{
			return Simd{ops_impl<T, N, default_isa>::div(a.m_native, b.m_native)};
		}

		MAG_INLINE friend Simd operator+(const Simd& a, T b)
			requires supports_add<T, N, default_isa>
		{
			return Simd{ops_impl<T, N, default_isa>::add(a.m_native,
														 ops_impl<T, N, default_isa>::splat(b))};
		}
		MAG_INLINE friend Simd operator+(T a, const Simd& b)
			requires supports_add<T, N, Isa>
		{
			return Simd{ops_impl<T, N, Isa>::add(ops_impl<T, N, Isa>::splat(a), b.m_native)};
		}
		MAG_INLINE friend Simd operator-(const Simd& a, T b)
			requires supports_sub<T, N, Isa>
		{
			return Simd{ops_impl<T, N, Isa>::sub(a.m_native, ops_impl<T, N, Isa>::splat(b))};
		}
		MAG_INLINE friend Simd operator-(T a, const Simd& b)
			requires supports_sub<T, N, Isa>
		{
			return Simd{ops_impl<T, N, Isa>::sub(ops_impl<T, N, Isa>::splat(a), b.m_native)};
		}
		MAG_INLINE friend Simd operator*(const Simd& a, T b)
			requires supports_mul<T, N, Isa>
		{
			return Simd{ops_impl<T, N, Isa>::mul(a.m_native, ops_impl<T, N, Isa>::splat(b))};
		}
		MAG_INLINE friend Simd operator*(T a, const Simd& b)
			requires supports_mul<T, N, Isa>
		{
			return Simd{ops_impl<T, N, Isa>::mul(ops_impl<T, N, Isa>::splat(a), b.m_native)};
		}
		MAG_INLINE friend Simd operator/(const Simd& a, T b)
			requires supports_div<T, N, Isa>
		{
			return Simd{ops_impl<T, N, Isa>::div(a.m_native, ops_impl<T, N, Isa>::splat(b))};
		}
		MAG_INLINE friend Simd operator/(T a, const Simd& b)
			requires supports_div<T, N, Isa>
		{
			return Simd{ops_impl<T, N, Isa>::div(ops_impl<T, N, Isa>::splat(a), b.m_native)};
		}

		MAG_INLINE Simd& operator+=(const Simd& o)
			requires supports_add<T, N, Isa>
		{
			m_native = ops_impl<T, N, Isa>::add(m_native, o.m_native);
			return *this;
		}
		MAG_INLINE Simd& operator-=(const Simd& o)
			requires supports_sub<T, N, Isa>
		{
			m_native = ops_impl<T, N, Isa>::sub(m_native, o.m_native);
			return *this;
		}
		MAG_INLINE Simd& operator*=(const Simd& o)
			requires supports_mul<T, N, Isa>
		{
			m_native = ops_impl<T, N, Isa>::mul(m_native, o.m_native);
			return *this;
		}
		MAG_INLINE Simd& operator/=(const Simd& o)
			requires supports_div<T, N, Isa>
		{
			m_native = ops_impl<T, N, Isa>::div(m_native, o.m_native);
			return *this;
		}
		MAG_INLINE Simd& operator+=(const T o)
			requires supports_add<T, N, Isa>
		{
			m_native = ops_impl<T, N, Isa>::add(m_native, ops_impl<T, N, Isa>::splat(o));
			return *this;
		}
		MAG_INLINE Simd& operator-=(const T o)
			requires supports_sub<T, N, Isa>
		{
			m_native = ops_impl<T, N, Isa>::sub(m_native, ops_impl<T, N, Isa>::splat(o));
			return *this;
		}
		MAG_INLINE Simd& operator*=(const T o)
			requires supports_mul<T, N, Isa>
		{
			m_native = ops_impl<T, N, Isa>::mul(m_native, ops_impl<T, N, Isa>::splat(o));
			return *this;
		}
		MAG_INLINE Simd& operator/=(const T o)
			requires supports_div<T, N, Isa>
		{
			m_native = ops_impl<T, N, Isa>::div(m_native, ops_impl<T, N, Isa>::splat(o));
			return *this;
		}

		[[nodiscard]] constexpr native_type native() const noexcept { return m_native; }

		MAG_INLINE void store(std::span<T, N> dst) const noexcept
		{
			ops_impl<T, N, Isa>::store(dst, m_native);
		}
		MAG_INLINE void store(T* dst) const noexcept { ops_impl<T, N, Isa>::store(dst, m_native); }
	};

	export template <Numeric T, typename Abi = native_abi>
	using basic_simd = Simd<T, abi_lanes_v<T, Abi>>;

	export template <Numeric T>
	using native_simd = basic_simd<T, native_abi>;

	export template <Numeric T>
	using scalar_simd = basic_simd<T, scalar_abi>;

	export template <Numeric T, size_t N, simd_isa Isa = default_isa>
	using fixed_simd = basic_simd<T, fixed_abi<N>>;

	export template <Numeric T, typename Abi = native_abi>
	[[nodiscard]] MAG_INLINE auto load(const T* ptr) noexcept -> basic_simd<T, Abi>
	{
		return basic_simd<T, Abi>{ptr};
	}

	export template <Numeric T, typename Abi = native_abi>
	[[nodiscard]] MAG_INLINE auto splat(T value) noexcept -> basic_simd<T, Abi>
	{
		return basic_simd<T, Abi>{value};
	}

	export template <typename T, size_t N, simd_isa Isa>
	T hsum(const Simd<T, N, Isa>& s)
		requires supports_reduction<T, N, Isa>
	{
		return ops_impl<T, N, Isa>::hsum(s.native());
	}

	export template <typename T, size_t N, simd_isa Isa>
	T hmin(const Simd<T, N, Isa>& s)
		requires supports_reduction<T, N, Isa>
	{
		return ops_impl<T, N, Isa>::hmin(s.native());
	}

	export template <typename T, size_t N, simd_isa Isa>
	T hmax(const Simd<T, N, Isa>& s)
		requires supports_reduction<T, N, Isa>
	{
		return ops_impl<T, N, Isa>::hmax(s.native());
	}

	export template <typename T, size_t N, simd_isa Isa>
	Simd<T, N, Isa> max(const Simd<T, N, Isa>& a, const Simd<T, N, Isa>& b) noexcept
		requires supports_max<T, N, Isa>
	{
		return Simd<T, N, Isa>{ops_impl<T, N, Isa>::max(a.native(), b.native())};
	}

	export template <typename T, size_t N, simd_isa Isa>
	Simd<T, N, Isa> min(const Simd<T, N, Isa>& a, const Simd<T, N, Isa>& b) noexcept
		requires supports_min<T, N, Isa>
	{
		return Simd<T, N, Isa>{ops_impl<T, N, Isa>::min(a.native(), b.native())};
	}

	export template <typename T, size_t N, simd_isa Isa>
	Simd<T, N, Isa> neg(const Simd<T, N, Isa>& v) noexcept
		requires supports_neg<T, N, Isa>
	{
		return Simd<T, N, Isa>{ops_impl<T, N, Isa>::neg(v.native())};
	}

	export template <typename T, size_t N, simd_isa Isa>
	Simd<T, N, Isa> abs(const Simd<T, N, Isa>& v) noexcept
		requires supports_abs<T, N, Isa>
	{
		return Simd<T, N, Isa>{ops_impl<T, N, Isa>::abs(v.native())};
	}

	export template <typename T, size_t N, simd_isa Isa>
	Simd<T, N, Isa> sqrt(const Simd<T, N, Isa>& v) noexcept
		requires supports_sqrt<T, N, Isa>
	{
		return Simd<T, N, Isa>{ops_impl<T, N, Isa>::sqrt(v.native())};
	}
} // namespace mag::simd
