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
#include <span>
export module mag.simd:sse3;

import :ops;
export import :sse2;

// NOLINTBEGIN(portability-simd-intrinsics)

namespace mag::simd
{
	template <>
	struct ops_impl<float, 4, simd_isa::sse3> : ops_impl<float, 4, simd_isa::sse2>
	{
		MAG_INLINE static float hsum(const native_t v) noexcept
		{
			return _mm_cvtss_f32(_mm_hadd_ps(v, v));
		}
	};

	template <>
	struct ops_impl<double, 2, simd_isa::sse3> : ops_impl<double, 2, simd_isa::sse2>
	{
		MAG_INLINE static double hsum(const native_t v) noexcept
		{
			return _mm_cvtsd_f64(_mm_hadd_pd(v, v));
		}
	};

	template <>
	struct ops_impl<int8_t, 16, simd_isa::sse3> : ops_impl<int8_t, 16, simd_isa::sse2>
	{
	};

	template <>
	struct ops_impl<int16_t, 8, simd_isa::sse3> : ops_impl<int16_t, 8, simd_isa::sse2>
	{
	};

	template <>
	struct ops_impl<int32_t, 4, simd_isa::sse3> : ops_impl<int32_t, 4, simd_isa::sse2>
	{
	};

	template <>
	struct ops_impl<int64_t, 2, simd_isa::sse3> : ops_impl<int64_t, 2, simd_isa::sse2>
	{
	};

	template <>
	struct ops_impl<uint8_t, 16, simd_isa::sse3> : ops_impl<uint8_t, 16, simd_isa::sse2>
	{
	};

	template <>
	struct ops_impl<uint16_t, 8, simd_isa::sse3> : ops_impl<uint16_t, 8, simd_isa::sse2>
	{
	};

	template <>
	struct ops_impl<uint32_t, 4, simd_isa::sse3> : ops_impl<uint32_t, 4, simd_isa::sse2>
	{
	};

	template <>
	struct ops_impl<uint64_t, 2, simd_isa::sse3> : ops_impl<uint64_t, 2, simd_isa::sse2>
	{
	};
} // namespace mag::simd

// NOLINTEND(portability-simd-intrinsics)
