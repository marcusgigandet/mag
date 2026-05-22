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
export module mag:simd;

export import :simd_concepts;
export import :simd_ops;

#ifdef MAG_NEON_SIMD
export import :neon_ops;
#elif MAG_X86_SIMD
export import :x86_ops;
#endif

import :concepts;

namespace mag
{
	/**
	 * @brief SIMD interface.
	 *
	 * Provides the primitive SIMD operations for a given type and width.
	 *
	 * @tparam T Scalar element type.
	 * @tparam N SIMD lane count.
	 */
	export template <Numeric T, size_t N>
	class Simd
	{
		/// Underlying SIMD register storage
		ops<T, N>::native_t m_data;

	public:
		Simd() noexcept = default;

		explicit Simd(const T data)
			requires(supports_splat<T, N>)
			: m_data(ops<T, N>::splat(data))
		{
		}

		explicit Simd(const std::span<T, N> data)
			requires(supports_splat<T, N>)
			: m_data(ops<T, N>::splat(data))
		{
		}

		explicit Simd(const T* ptr)
			requires(loadable<T, N>)
			: m_data(ops<T, N>::load(ptr))
		{
		}

		template <typename... Args>
			requires(sizeof...(Args) == N) && (std::same_as<Args, T> && ...)
		explicit Simd(Args... args) noexcept
		{
			alignas(alignof(T)) T tmp[N]{static_cast<T>(args)...};
			m_data = ops<T, N>::load(tmp);
		}

		explicit Simd(const ops<T, N>::native_t data) : m_data(data) {}

		Simd(const Simd&) noexcept = default;
		Simd(Simd&&) noexcept = default;
		Simd& operator=(const Simd&) noexcept = default;
		Simd& operator=(Simd&&) noexcept = default;

		MAG_INLINE friend Simd operator+(const Simd a, const Simd b)
			requires supports_add<T, N>
		{
			return Simd{ops<T, N>::add(a.m_data, b.m_data)};
		}
		MAG_INLINE friend Simd operator-(const Simd a, const Simd b)
			requires supports_sub<T, N>
		{
			return Simd{ops<T, N>::sub(a.m_data, b.m_data)};
		}
		MAG_INLINE friend Simd operator*(const Simd a, const Simd b)
			requires supports_mul<T, N>
		{
			return Simd{ops<T, N>::mul(a.m_data, b.m_data)};
		}
		MAG_INLINE friend Simd operator/(const Simd a, const Simd b)
			requires supports_div<T, N>
		{
			return Simd{ops<T, N>::div(a.m_data, b.m_data)};
		}

		MAG_INLINE Simd operator+=(const Simd o)
			requires supports_sub<T, N>
		{
			return m_data = *this + o;
		}
		MAG_INLINE Simd operator-=(const Simd o)
			requires supports_sub<T, N>
		{
			return m_data = *this - o;
		}
		MAG_INLINE Simd operator*=(const Simd o)
			requires supports_sub<T, N>
		{
			return m_data = *this * o;
		}
		MAG_INLINE Simd operator/=(const Simd o)
			requires supports_sub<T, N>
		{
			return m_data = *this / o;
		}

		/**
		 * @brief Reduces a SIMD register to a single scalar by summing all lanes.
		 *
		 * @return Sum of all elements in v.
		 */
		MAG_INLINE T hsum() const
			requires supports_hsum<T, N>
		{
			return ops<T, N>::hsum(m_data);
		}

		/**
		 * @brief Broadcasts a scalar value across all SIMD lanes.
		 *
		 * @param s Scalar value to replicate.
		 */
		MAG_INLINE void splat(T s) noexcept { m_data = ops<T, N>::splat(s); }

		/**
		 * @brief Loads N elements from memory into a SIMD register.
		 *
		 * @param data Span of data containing data to load into SIMD register.
		 */
		MAG_INLINE void load(const std::span<T, N> data) { m_data = ops<T, N>::load(data.data()); }

		MAG_INLINE void load(const T* ptr) { m_data = ops<T, N>::load(ptr); }

		template <typename... Args>
			requires(sizeof...(Args) == N) && (std::same_as<Args, T> && ...)
		MAG_INLINE void load(Args... args) noexcept
		{
			alignas(alignof(T)) T tmp[N]{static_cast<T>(args)...};
			m_data = ops<T, N>::load(tmp);
		}

		/**
		 * @brief Stores SIMD register values back to memory.
		 *
		 * @param dst Span of data to write SIMD registers to.
		 */
		MAG_INLINE void store(std::span<T, N> dst) const { ops<T, N>::store(dst, m_data); }

		/**
		 * @brief Stores SIMD register values back to memory.
		 *
		 * @param dst Span of data to write SIMD registers to.
		 */
		MAG_INLINE void store(T* dst) const { ops<T, N>::store(dst, m_data); }
	};
} // namespace mag
