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
#include <arm_neon.h>
export module mag:neon_traits;

import :simd_traits;

namespace mag
{
	// clang-format off
#ifdef MAG_ENABLE_SIMD_EXTENDED
	// Division is unsupported
	template <> struct simd_traits<int8_t, 16> :  // Vec<int8_t, 16>
				simd_traits_impl<int8x16_t, true, true, true, true, false> {};
	template <> struct simd_traits<int16_t, 8> :  // Vec<int16_t, 8>
				simd_traits_impl<int16x8_t, true, true, true, true, false> {};
	template <> struct simd_traits<int32_t, 4> :  // Vec<int32_t, 4>
				simd_traits_impl<int32x4_t, true, true, true, true, false> {};
	template <> struct simd_traits<uint8_t, 16> : // Vec<uint8_t, 16>
				simd_traits_impl<uint8x16_t, true, true, true, true, false> {};
	template <> struct simd_traits<uint16_t, 8> : // Vec<uint16_t, 8>
				simd_traits_impl<uint16x8_t, true, true, true, true, false> {};
	template <> struct simd_traits<uint32_t, 4> : // Vec<uint32_t, 4>
				simd_traits_impl<uint32x4_t, true, true, true, true, false> {};

	// Division and multiplication are unsupported
	template <> struct simd_traits<int64_t, 2> :  // Vec<int64_t, 2>
				simd_traits_impl<int64x2_t, true, true, true, false, false> {};
	template <> struct simd_traits<uint64_t, 2> : // Vec<uint64_t, 2>
				simd_traits_impl<uint64x2_t, true, true, true, false, false> {};
#endif
	// Division and multiplication are supported
	template <> struct simd_traits<float, 4> :  // Vec<float, 4>
				simd_traits_impl<float32x4_t, true, true, true, true, true> {};
	template <> struct simd_traits<double, 2> : // Vec<double, 2>
				simd_traits_impl<float64x2_t, true, true, true, true, true> {};
	// clang-format on
} // namespace mag
