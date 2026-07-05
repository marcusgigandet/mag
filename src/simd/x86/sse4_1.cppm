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
#include <immintrin.h>
export module mag.simd:sse4_1;

import :ops;
export import :sse3;

// NOLINTBEGIN(portability-simd-intrinsics)

namespace mag::simd
{
	template <>
	struct ops_impl<float, 4, simd_isa::sse4_1> : ops_impl<float, 4, simd_isa::sse3>
	{
	};

	template <>
	struct ops_impl<double, 2, simd_isa::sse4_1> : ops_impl<double, 2, simd_isa::sse3>
	{
	};

	template <>
	struct ops_impl<int8_t, 16, simd_isa::sse4_1> : ops_impl<int8_t, 16, simd_isa::sse3>
	{
	};

	template <>
	struct ops_impl<int16_t, 8, simd_isa::sse4_1> : ops_impl<int16_t, 8, simd_isa::sse3>
	{
	};

	template <>
	struct ops_impl<int32_t, 4, simd_isa::sse4_1> : ops_impl<int32_t, 4, simd_isa::sse3>
	{
		MAG_INLINE static native_t min(const native_t a, const native_t b) noexcept
		{
			return _mm_min_epi32(a, b);
		}
	};

	template <>
	struct ops_impl<int64_t, 2, simd_isa::sse4_1> : ops_impl<int64_t, 2, simd_isa::sse3>
	{
	};

	template <>
	struct ops_impl<uint8_t, 16, simd_isa::sse4_1> : ops_impl<uint8_t, 16, simd_isa::sse3>
	{
	};

	template <>
	struct ops_impl<uint16_t, 8, simd_isa::sse4_1> : ops_impl<uint16_t, 8, simd_isa::sse3>
	{
	};

	template <>
	struct ops_impl<uint32_t, 4, simd_isa::sse4_1> : ops_impl<uint32_t, 4, simd_isa::sse3>
	{
		MAG_INLINE static uint32_t hmax(const native_t v) noexcept
		{
			__m128i max = v;
			max = _mm_max_epi32(max, _mm_srli_si128(max, 8));
			max = _mm_max_epi32(max, _mm_srli_si128(max, 4));
			return static_cast<uint32_t>(_mm_cvtsi128_si32(max));
		}

		MAG_INLINE static uint32_t hmin(const native_t v) noexcept
		{
			__m128i min = v;
			min = _mm_min_epi32(min, _mm_srli_si128(min, 8));
			min = _mm_min_epi32(min, _mm_srli_si128(min, 4));
			return static_cast<uint32_t>(_mm_cvtsi128_si32(min));
		}

		MAG_INLINE static native_t min(const native_t a, const native_t b) noexcept
		{
			return _mm_min_epu32(a, b);
		}
	};

	template <>
	struct ops_impl<uint64_t, 2, simd_isa::sse4_1> : ops_impl<uint64_t, 2, simd_isa::sse3>
	{
	};
} // namespace mag::simd

// NOLINTEND(portability-simd-intrinsics)
