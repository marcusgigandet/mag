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
export module mag.simd:ssse3;

import :ops;
export import :sse2;

// NOLINTBEGIN(portability-simd-intrinsics)

namespace MAG_NAMESPACE::simd
{
	template <Numeric T, std::size_t N>
	struct ops_impl<T, N, simd_isa::ssse3> : ops_impl<T, N, simd_isa::sse2>
	{
	};

	template <>
	struct ops_impl<float, 4, simd_isa::ssse3> : ops_impl<float, 4, simd_isa::sse2>
	{
		MAG_INLINE static float hsum(const native_t v) noexcept
		{
			return _mm_cvtss_f32(_mm_hadd_ps(_mm_hadd_ps(v, v), v));
		}
	};

	template <>
	struct ops_impl<double, 2, simd_isa::ssse3> : ops_impl<double, 2, simd_isa::sse2>
	{
		MAG_INLINE static double hsum(const native_t v) noexcept
		{
			return _mm_cvtsd_f64(_mm_hadd_pd(v, v));
		}
	};

	template <>
	struct ops_impl<int32_t, 4, simd_isa::ssse3> : ops_impl<int32_t, 4, simd_isa::sse2>
	{
		MAG_INLINE static int32_t hsum(const native_t v) noexcept
		{
			return _mm_cvtsi128_si32(_mm_hadd_epi32(_mm_hadd_epi32(v, v), v));
		}
	};

	template <>
	struct ops_impl<uint32_t, 4, simd_isa::ssse3> : ops_impl<uint32_t, 4, simd_isa::sse2>
	{
		MAG_INLINE static uint32_t hsum(const native_t v) noexcept
		{
			return static_cast<uint32_t>(
					_mm_cvtsi128_si32(_mm_hadd_epi32(_mm_hadd_epi32(v, v), v)));
		}
	};
} // namespace MAG_NAMESPACE::simd

// NOLINTEND(portability-simd-intrinsics)
