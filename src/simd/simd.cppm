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
		/// Underlying SIMD register storage
		ops<T, N>::native_t m_native{};

	public:
		MAG_INLINE Simd() noexcept = default;

		/**
		 * @brief Broadcasts a scalar value across all SIMD lanes.
		 *
		 * @param s Scalar value to replicate.
		 */
		template <std::convertible_to<T> U>
		MAG_INLINE explicit Simd(U s) : m_native(ops<T, N>::splat(static_cast<T>(s)))
		{
		}

		/**
		 * @brief Uploads a span of N elements to the SIMD register.
		 *
		 * @param data Data being uploaded.
		 */
		MAG_INLINE explicit Simd(const std::span<T, N> data) :
			m_native(ops<T, N>::load(data.data()))
		{
		}

		/**
		 * @brief Uploads N elements from memory into a SIMD register
		 *
		 * @param ptr Must be a valid pointer containing N elements of T.
		 *
		 * @note This method is unsafe and will result in an error if an incorrectly sized ptr is
		 * provided.
		 */
		MAG_INLINE explicit Simd(const T* ptr) : m_native(ops<T, N>::load(ptr)) {}

		/**
		 * @brief Loads N unique elements into a SIMD register.
		 *
		 * Initializes each lane of the SIMD register with the corresponding
		 * argument value.
		 *
		 * @tparam Args Types of the values used to initialize the SIMD lanes.
		 * @param args Values to load into the SIMD register lanes.
		 */
		template <typename... Args>
			requires(sizeof...(Args) == N) && (std::convertible_to<Args, T> && ...)
		MAG_INLINE explicit Simd(Args... args) noexcept
		{
			alignas(alignof(T)) T tmp[N]{static_cast<T>(args)...};
			m_native = ops<T, N>::load(tmp);
		}

		/**
		 * @brief Copies data from a native simd type.
		 *
		 * @param data Native simd type being copied.
		 */
		MAG_INLINE explicit Simd(const ops<T, N>::native_t data) : m_native(data) {}

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
			return Simd{a.m_native + ops<T, N>::splat(b)};
		}
		MAG_INLINE friend Simd operator+(T a, const Simd& b)
			requires supports_add<T, N>
		{
			return Simd{ops<T, N>::splat(a) + b.m_native};
		}
		MAG_INLINE friend Simd operator-(const Simd& a, T b)
			requires supports_sub<T, N>
		{
			return Simd{a.m_native - ops<T, N>::splat(b)};
		}
		MAG_INLINE friend Simd operator-(T a, const Simd& b)
			requires supports_sub<T, N>
		{
			return Simd{ops<T, N>::splat(a) - b.m_native};
		}
		MAG_INLINE friend Simd operator*(const Simd& a, T b)
			requires supports_mul<T, N>
		{
			return Simd{a.m_native * ops<T, N>::splat(b)};
		}
		MAG_INLINE friend Simd operator*(T a, const Simd& b)
			requires supports_mul<T, N>
		{
			return Simd{ops<T, N>::splat(a) * b.m_native};
		}
		MAG_INLINE friend Simd operator/(const Simd& a, T b)
			requires supports_div<T, N>
		{
			return Simd{a.m_native / ops<T, N>::splat(b)};
		}
		MAG_INLINE friend Simd operator/(T a, const Simd& b)
			requires supports_div<T, N>
		{
			return Simd{ops<T, N>::splat(a) / b.m_native};
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

		[[nodiscard]] constexpr ops<T, N>::native_t native() const noexcept { return m_native; }

		/**
		 * @brief Stores SIMD register values back to memory.
		 *
		 * @param dst Span of data to write SIMD registers to.
		 */
		MAG_INLINE void store(std::span<T, N> dst) const { ops<T, N>::store(dst, m_native); }

		/**
		 * @brief Stores SIMD register values back to memory.
		 *
		 * @param dst Data to write SIMD registers to.
		 */
		MAG_INLINE void store(T* dst) const { ops<T, N>::store(dst, m_native); }
	};

	/**
	 * @brief Reduces all SIMD lanes to a single scalar by summing elements.
	 *
	 * @tparam T Scalar element type.
	 * @tparam N SIMD lane count.
	 *
	 * @param s SIMD value to reduce.
	 *
	 * @return Sum of all elements in the SIMD register.
	 */
	export template <typename T, size_t N>
	T hsum(const Simd<T, N>& s)
		requires supports_reduction<T, N>
	{
		return ops<T, N>::hsum(s.native());
	}

	/**
	 * @brief Reduces all SIMD lanes to a single scalar by selecting the minimum value.
	 *
	 * @tparam T Scalar element type.
	 * @tparam N SIMD lane count.
	 *
	 * @param s SIMD value to reduce.
	 *
	 * @return Smallest element in the SIMD register.
	 */
	export template <typename T, size_t N>
	T hmin(const Simd<T, N>& s)
		requires supports_reduction<T, N>
	{
		return ops<T, N>::hmin(s.native());
	}

	/**
	 * @brief Reduces all SIMD lanes to a single scalar by selecting the maximum value.
	 *
	 * @tparam T Scalar element type.
	 * @tparam N SIMD lane count.
	 *
	 * @param s SIMD value to reduce.
	 *
	 * @return Largest element in the SIMD register.
	 */
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
