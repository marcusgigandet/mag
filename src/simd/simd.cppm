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
#include <cstdint>
#include <span>
export module mag:simd;

export import :simd_abi;
export import :simd_concepts;
export import :simd_ops;

#ifdef MAG_NEON_SIMD
export import :neon_ops;
#elif MAG_X86_SIMD
export import :x86_ops;
#endif

import :concepts;

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
	export template <Numeric T, size_t N>
	class Simd
	{
		using native_type = ops<T, N>::native_t;

		/// Underlying SIMD register storage
		native_type m_native{};

	public:
		using value_type = T;
		static constexpr size_t lanes = N;

		[[nodiscard]] static consteval size_t size() noexcept { return N; }

		MAG_INLINE Simd() noexcept = default;

		template <std::convertible_to<T> U>
		MAG_INLINE explicit Simd(U s) noexcept : m_native(ops<T, N>::splat(static_cast<T>(s)))
		{
		}

		MAG_INLINE explicit Simd(const std::span<const T, N> data) noexcept :
			m_native(ops<T, N>::load(data.data()))
		{
		}

		MAG_INLINE explicit Simd(const T* ptr) noexcept : m_native(ops<T, N>::load(ptr)) {}

		template <typename... Args>
			requires(sizeof...(Args) == N) && (std::convertible_to<Args, T> && ...)
		MAG_INLINE explicit Simd(Args... args) noexcept
		{
			alignas(sizeof(native_type)) T tmp[N]{static_cast<T>(args)...};
			m_native = ops<T, N>::load(tmp);
		}

		MAG_INLINE explicit Simd(native_type data) noexcept : m_native(data) {}

		Simd(const Simd&) noexcept = default;
		Simd(Simd&&) noexcept = default;
		Simd& operator=(const Simd&) noexcept = default;
		Simd& operator=(Simd&&) noexcept = default;

		MAG_INLINE friend Simd operator+(const Simd& a, const Simd& b)
			requires supports_add<T, N>
		{
			return Simd{ops<T, N>::add(a.m_native, b.m_native)};
		}
		MAG_INLINE friend Simd operator-(const Simd& a, const Simd& b)
			requires supports_sub<T, N>
		{
			return Simd{ops<T, N>::sub(a.m_native, b.m_native)};
		}
		MAG_INLINE friend Simd operator*(const Simd& a, const Simd& b)
			requires supports_mul<T, N>
		{
			return Simd{ops<T, N>::mul(a.m_native, b.m_native)};
		}
		MAG_INLINE friend Simd operator/(const Simd& a, const Simd& b)
			requires supports_div<T, N>
		{
			return Simd{ops<T, N>::div(a.m_native, b.m_native)};
		}

		MAG_INLINE friend Simd operator+(const Simd& a, T b)
			requires supports_add<T, N>
		{
			return Simd{ops<T, N>::add(a.m_native, ops<T, N>::splat(b))};
		}
		MAG_INLINE friend Simd operator+(T a, const Simd& b)
			requires supports_add<T, N>
		{
			return Simd{ops<T, N>::add(ops<T, N>::splat(a), b.m_native)};
		}
		MAG_INLINE friend Simd operator-(const Simd& a, T b)
			requires supports_sub<T, N>
		{
			return Simd{ops<T, N>::sub(a.m_native, ops<T, N>::splat(b))};
		}
		MAG_INLINE friend Simd operator-(T a, const Simd& b)
			requires supports_sub<T, N>
		{
			return Simd{ops<T, N>::sub(ops<T, N>::splat(a), b.m_native)};
		}
		MAG_INLINE friend Simd operator*(const Simd& a, T b)
			requires supports_mul<T, N>
		{
			return Simd{ops<T, N>::mul(a.m_native, ops<T, N>::splat(b))};
		}
		MAG_INLINE friend Simd operator*(T a, const Simd& b)
			requires supports_mul<T, N>
		{
			return Simd{ops<T, N>::mul(ops<T, N>::splat(a), b.m_native)};
		}
		MAG_INLINE friend Simd operator/(const Simd& a, T b)
			requires supports_div<T, N>
		{
			return Simd{ops<T, N>::div(a.m_native, ops<T, N>::splat(b))};
		}
		MAG_INLINE friend Simd operator/(T a, const Simd& b)
			requires supports_div<T, N>
		{
			return Simd{ops<T, N>::div(ops<T, N>::splat(a), b.m_native)};
		}

		MAG_INLINE Simd& operator+=(const Simd& o)
			requires supports_add<T, N>
		{
			m_native = ops<T, N>::add(m_native, o.m_native);
			return *this;
		}
		MAG_INLINE Simd& operator-=(const Simd& o)
			requires supports_sub<T, N>
		{
			m_native = ops<T, N>::sub(m_native, o.m_native);
			return *this;
		}
		MAG_INLINE Simd& operator*=(const Simd& o)
			requires supports_mul<T, N>
		{
			m_native = ops<T, N>::mul(m_native, o.m_native);
			return *this;
		}
		MAG_INLINE Simd& operator/=(const Simd& o)
			requires supports_div<T, N>
		{
			m_native = ops<T, N>::div(m_native, o.m_native);
			return *this;
		}
		MAG_INLINE Simd& operator+=(const T o)
			requires supports_add<T, N>
		{
			m_native = ops<T, N>::add(m_native, ops<T, N>::splat(o));
			return *this;
		}
		MAG_INLINE Simd& operator-=(const T o)
			requires supports_sub<T, N>
		{
			m_native = ops<T, N>::sub(m_native, ops<T, N>::splat(o));
			return *this;
		}
		MAG_INLINE Simd& operator*=(const T o)
			requires supports_mul<T, N>
		{
			m_native = ops<T, N>::mul(m_native, ops<T, N>::splat(o));
			return *this;
		}
		MAG_INLINE Simd& operator/=(const T o)
			requires supports_div<T, N>
		{
			m_native = ops<T, N>::div(m_native, ops<T, N>::splat(o));
			return *this;
		}

		[[nodiscard]] constexpr native_type native() const noexcept { return m_native; }

		MAG_INLINE void store(std::span<T, N> dst) const noexcept
		{
			ops<T, N>::store(dst, m_native);
		}
		MAG_INLINE void store(T* dst) const noexcept { ops<T, N>::store(dst, m_native); }
	};

	export template <Numeric T, typename Abi = native_abi>
	using basic_simd = Simd<T, abi_lanes_v<T, Abi>>;

	export template <Numeric T>
	using native_simd = basic_simd<T, native_abi>;

	export template <Numeric T>
	using scalar_simd = basic_simd<T, scalar_abi>;

	export template <Numeric T, size_t N>
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

	export template <typename T, size_t N>
	T hsum(const Simd<T, N>& s)
		requires supports_reduction<T, N>
	{
		return ops<T, N>::hsum(s.native());
	}

	export template <typename T, size_t N>
	T hmin(const Simd<T, N>& s)
		requires supports_reduction<T, N>
	{
		return ops<T, N>::hmin(s.native());
	}

	export template <typename T, size_t N>
	T hmax(const Simd<T, N>& s)
		requires supports_reduction<T, N>
	{
		return ops<T, N>::hmax(s.native());
	}

	export template <Numeric T, size_t N>
	Simd<T, N> max(const Simd<T, N>& a, const Simd<T, N>& b) noexcept
		requires supports_max<T, N>
	{
		return Simd<T, N>{ops<T, N>::max(a.native(), b.native())};
	}

	export template <Numeric T, size_t N>
	Simd<T, N> min(const Simd<T, N>& a, const Simd<T, N>& b) noexcept
		requires supports_min<T, N>
	{
		return Simd<T, N>{ops<T, N>::min(a.native(), b.native())};
	}

	export template <Numeric T, size_t N>
	Simd<T, N> neg(const Simd<T, N>& v) noexcept
		requires supports_neg<T, N>
	{
		return Simd<T, N>{ops<T, N>::neg(v.native())};
	}

	export template <Numeric T, size_t N>
	Simd<T, N> abs(const Simd<T, N>& v) noexcept
		requires supports_abs<T, N>
	{
		return Simd<T, N>{ops<T, N>::abs(v.native())};
	}

	export template <Numeric T, size_t N>
	Simd<T, N> sqrt(const Simd<T, N>& v) noexcept
		requires supports_sqrt<T, N>
	{
		return Simd<T, N>{ops<T, N>::sqrt(v.native())};
	}
} // namespace mag::simd
