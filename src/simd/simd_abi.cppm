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
export module mag.simd:abi;

import :concepts;

namespace MAG_NAMESPACE::simd
{
	/**
	 * @brief Tag type for scalar (single-lane) ABI.
	 *
	 * When used as the ABI parameter, indicates that operations should be performed
	 * on a single element at a time (non-SIMD behavior). This is the fallback for
	 * cases where vectorization is not available or not desired.
	 */
	export struct scalar_abi
	{
	};

	/**
	 * @brief Tag type for native SIMD ABI.
	 *
	 * Uses the widest available SIMD register on the target architecture (e.g., 128-bit
	 * SSE/NEON, 256-bit AVX). The number of lanes adapts based on the element type:
	 * for float on 128-bit hardware, 4 lanes; for double, 2 lanes; etc.
	 */
	export struct native_abi
	{
	};

	/**
	 * @brief Tag type for fixed-width SIMD ABI.
	 *
	 * Specifies a fixed number of lanes regardless of element type or target architecture.
	 * For example, `fixed_abi<4>` always holds exactly 4 elements.
	 *
	 * @tparam N Number of lanes.
	 */
	export template <size_t N>
	struct fixed_abi
	{
		static constexpr size_t lanes = N;
	};

	/**
	 * @brief Compile-time query for the native register width in bytes.
	 *
	 * @return Native register width in bytes (compile-time constant).
	 */
	consteval size_t nativeRegisterBytes() noexcept
	{
#if defined(MAG_SIMD_BACKEND_SSE2) || defined(MAG_SIMD_BACKEND_SSSE3) ||                           \
		defined(MAG_SIMD_BACKEND_SSE4_1) || defined(MAG_SIMD_BACKEND_NEON)
		return 16;
#else
		return 0;
#endif
	}

	/**
	 * @brief Compute the default number of lanes for a given element type.
	 *
	 * Calculates the number of lanes that fit in the native register:
	 * - For a 128-bit register and `float` (4 bytes): 4 lanes
	 * - For a 128-bit register and `double` (8 bytes): 2 lanes
	 * - Returns 1 if no SIMD support is available
	 *
	 * @tparam T Element type (must be Numeric).
	 * @return Number of lanes for type T in the native register (compile-time constant).
	 */
	template <Numeric T>
	consteval size_t defaultLanes() noexcept
	{
		constexpr size_t bytes = nativeRegisterBytes();
		if constexpr (bytes == 0)
		{
			return 1;
		}
		else
		{
			constexpr size_t lanes = bytes / sizeof(T);
			return lanes == 0 ? 1 : lanes;
		}
	}

	/**
	 * @brief Type trait: query the number of lanes for a given type and ABI.
	 *
	 * Specializations determine how many SIMD lanes are active for a given element
	 * type and ABI tag:
	 * - `abi_lanes<T, scalar_abi>`: always 1
	 * - `abi_lanes<T, native_abi>`: depends on T and architecture
	 * - `abi_lanes<T, fixed_abi<N>>`: always N
	 *
	 * @tparam T Element type.
	 * @tparam Abi ABI tag (scalar_abi, native_abi, or fixed_abi<N>).
	 */
	template <Numeric T, typename Abi>
	struct abi_lanes;

	/**
	 * @brief Specialization: scalar ABI always has 1 lane.
	 *
	 * @tparam T Element type.
	 */
	template <Numeric T>
	struct abi_lanes<T, scalar_abi> : std::integral_constant<size_t, 1>
	{
	};

	/**
	 * @brief Specialization: native ABI lane count depends on T and architecture.
	 *
	 * @tparam T Element type.
	 */
	template <Numeric T>
	struct abi_lanes<T, native_abi> : std::integral_constant<size_t, defaultLanes<T>()>
	{
	};

	/**
	 * @brief Specialization: fixed ABI has N lanes.
	 *
	 * @tparam T Element type.
	 * @tparam N Fixed number of lanes.
	 */
	template <Numeric T, size_t N>
	struct abi_lanes<T, fixed_abi<N>> : std::integral_constant<size_t, N>
	{
	};

	/**
	 * @brief Helper variable template: get the number of lanes for a type and ABI.
	 *
	 * Provides a convenient way to query lane counts at compile time:
	 * ```cpp
	 * constexpr size_t nativeFloatLanes = abi_lanes_v<float, native_abi>;   // 4 on 128-bit
	 * constexpr size_t fixed4Lanes = abi_lanes_v<double, fixed_abi<4>>;     // always 4
	 * constexpr size_t scalarLanes = abi_lanes_v<int, scalar_abi>;          // always 1
	 * ```
	 *
	 * @tparam T Element type.
	 * @tparam Abi ABI tag.
	 */
	export template <Numeric T, typename Abi = native_abi>
	inline constexpr size_t abi_lanes_v = abi_lanes<T, Abi>::value;
} // namespace MAG_NAMESPACE::simd
