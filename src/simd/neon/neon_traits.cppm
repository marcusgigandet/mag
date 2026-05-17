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

export namespace mag::simd
{
#ifdef MAG_ENABLE_SIMD_EXTENDED
	// Vec<int8_t, 16>
	template <>
	struct traits<int8_t, 16> : traits_impl<int8x16_t>
	{
	};

	// Vec<int16_t, 8>
	template <>
	struct traits<int16_t, 8> : traits_impl<int16x8_t>
	{
	};

	// Vec<int32_t, 4>
	template <>
	struct traits<int32_t, 4> : traits_impl<int32x4_t>
	{
	};

	// Vec<uint8_t, 16>
	template <>
	struct traits<uint8_t, 16> : traits_impl<uint8x16_t>
	{
	};

	// Vec<uint16_t, 8>
	template <>
	struct traits<uint16_t, 8> : traits_impl<uint16x8_t>
	{
	};

	// Vec<uint32_t, 4>
	template <>
	struct traits<uint32_t, 4> : traits_impl<uint32x4_t>
	{
	};

	// Vec<int64_t, 2>
	template <>
	struct traits<int64_t, 2> : traits_impl<int64x2_t>
	{
	};

	// Vec<uint64_t, 2>
	template <>
	struct traits<uint64_t, 2> : traits_impl<uint64x2_t>
	{
	};
#endif

	// Vec<float, 4>
	template <>
	struct traits<float, 4> : traits_impl<float32x4_t>
	{
	};

	// Vec<double, 2>
	template <>
	struct traits<double, 2> : traits_impl<float64x2_t>
	{
	};
} // namespace mag::simd
