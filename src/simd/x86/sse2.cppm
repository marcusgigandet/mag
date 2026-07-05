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
export module mag.simd:sse2;

import :ops;

// NOLINTBEGIN(portability-simd-intrinsics)

namespace mag::simd
{
	template <>
	struct ops_impl<float, 4, simd_isa::sse2>
	{
		using native_t = __m128;

		MAG_INLINE static native_t load(const float* p) noexcept { return _mm_loadu_ps(p); }
		MAG_INLINE static void store(float* p, const native_t v) noexcept { _mm_storeu_ps(p, v); }
		MAG_INLINE static void store(std::span<float, 4> dst, const native_t v) noexcept
		{
			_mm_storeu_ps(dst.data(), v);
		}

		MAG_INLINE static native_t splat(const float s) noexcept { return _mm_set1_ps(s); }

		MAG_INLINE static native_t add(const native_t a, const native_t b) noexcept
		{
			return _mm_add_ps(a, b);
		}
		MAG_INLINE static native_t sub(const native_t a, const native_t b) noexcept
		{
			return _mm_sub_ps(a, b);
		}
		MAG_INLINE static native_t mul(const native_t a, const native_t b) noexcept
		{
			return _mm_mul_ps(a, b);
		}
		MAG_INLINE static native_t div(const native_t a, const native_t b) noexcept
		{
			return _mm_div_ps(a, b);
		}

		MAG_INLINE static float hmax(const native_t v) noexcept
		{
			__m128 max1 = _mm_max_ps(v, _mm_movehl_ps(v, v));
			const __m128 max2 =
					_mm_max_ps(max1, _mm_shuffle_ps(max1, max1, _MM_SHUFFLE(1, 1, 1, 1)));
			return _mm_cvtss_f32(max2);
		}

		MAG_INLINE static float hmin(const native_t v) noexcept
		{
			__m128 min1 = _mm_min_ps(v, _mm_movehl_ps(v, v));
			const __m128 min2 =
					_mm_min_ps(min1, _mm_shuffle_ps(min1, min1, _MM_SHUFFLE(1, 1, 1, 1)));
			return _mm_cvtss_f32(min2);
		}

		MAG_INLINE static native_t max(const native_t a, const native_t b) noexcept
		{
			return _mm_max_ps(a, b);
		}

		MAG_INLINE static native_t min(const native_t a, const native_t b) noexcept
		{
			return _mm_min_ps(a, b);
		}

		MAG_INLINE static native_t neg(const native_t v) noexcept
		{
			return _mm_xor_ps(v, _mm_set1_ps(-0.0f));
		}

		MAG_INLINE static native_t abs(const native_t v) noexcept
		{
			return _mm_andnot_ps(_mm_set1_ps(-0.0f), v);
		}

		MAG_INLINE static native_t sqrt(const native_t v) noexcept { return _mm_sqrt_ps(v); }

		MAG_INLINE static float hsum(const native_t v) noexcept
		{
			__m128 t = _mm_add_ps(v, _mm_movehl_ps(v, v));
			t = _mm_add_ss(t, _mm_shuffle_ps(t, t, 1));
			return _mm_cvtss_f32(t);
		}
	};

	template <>
	struct ops_impl<double, 2, simd_isa::sse2>
	{
		using native_t = __m128d;

		MAG_INLINE static native_t load(const double* p) noexcept { return _mm_loadu_pd(p); }
		MAG_INLINE static void store(double* p, const native_t v) noexcept { _mm_storeu_pd(p, v); }
		MAG_INLINE static void store(std::span<double, 2> dst, const native_t v) noexcept
		{
			_mm_storeu_pd(dst.data(), v);
		}

		MAG_INLINE static native_t splat(const double s) noexcept { return _mm_set1_pd(s); }

		MAG_INLINE static native_t add(const native_t a, const native_t b) noexcept
		{
			return _mm_add_pd(a, b);
		}
		MAG_INLINE static native_t sub(const native_t a, const native_t b) noexcept
		{
			return _mm_sub_pd(a, b);
		}
		MAG_INLINE static native_t mul(const native_t a, const native_t b) noexcept
		{
			return _mm_mul_pd(a, b);
		}
		MAG_INLINE static native_t div(const native_t a, const native_t b) noexcept
		{
			return _mm_div_pd(a, b);
		}

		MAG_INLINE static double hmax(const native_t v) noexcept
		{
			const __m128d shifted = _mm_unpackhi_pd(v, v);
			const __m128d max = _mm_max_sd(v, shifted);
			return _mm_cvtsd_f64(max);
		}

		MAG_INLINE static double hmin(const native_t v) noexcept
		{
			const __m128d shifted = _mm_unpackhi_pd(v, v);
			const __m128d min = _mm_min_sd(v, shifted);
			return _mm_cvtsd_f64(min);
		}

		MAG_INLINE static native_t max(const native_t a, const native_t b) noexcept
		{
			return _mm_max_pd(a, b);
		}

		MAG_INLINE static native_t min(const native_t a, const native_t b) noexcept
		{
			return _mm_min_pd(a, b);
		}

		MAG_INLINE static native_t neg(const native_t v) noexcept
		{
			return _mm_xor_pd(v, _mm_set1_pd(-0.0));
		}

		MAG_INLINE static native_t abs(const native_t v) noexcept
		{
			return _mm_andnot_pd(_mm_set1_pd(-0.0), v);
		}

		MAG_INLINE static native_t sqrt(const native_t v) noexcept { return _mm_sqrt_pd(v); }
	};

	template <>
	struct ops_impl<int8_t, 16, simd_isa::sse2>
	{
		using native_t = __m128i;

		MAG_INLINE static native_t load(const int8_t* p) noexcept
		{
			return _mm_loadu_si128(reinterpret_cast<const __m128i*>(p));
		}
		MAG_INLINE static void store(int8_t* p, const native_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(p), v);
		}
		MAG_INLINE static void store(std::span<int8_t, 16> dst, const native_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(dst.data()), v);
		}

		MAG_INLINE static native_t splat(const int8_t s) noexcept { return _mm_set1_epi8(s); }

		MAG_INLINE static native_t add(const native_t a, const native_t b) noexcept
		{
			return _mm_add_epi8(a, b);
		}
		MAG_INLINE static native_t sub(const native_t a, const native_t b) noexcept
		{
			return _mm_sub_epi8(a, b);
		}
		MAG_INLINE static native_t mul(const native_t a, const native_t b) noexcept
		{
			const __m128i signA = _mm_cmpgt_epi8(_mm_setzero_si128(), a);
			const __m128i signB = _mm_cmpgt_epi8(_mm_setzero_si128(), b);
			const __m128i aLo = _mm_unpacklo_epi8(a, signA);
			const __m128i aHi = _mm_unpackhi_epi8(a, signA);
			const __m128i bLo = _mm_unpacklo_epi8(b, signB);
			const __m128i bHi = _mm_unpackhi_epi8(b, signB);
			const __m128i mulLo = _mm_mullo_epi16(aLo, bLo);
			const __m128i mulHi = _mm_mullo_epi16(aHi, bHi);
			const __m128i mask = _mm_set1_epi16(0x00FF);
			const __m128i loBytes = _mm_and_si128(mulLo, mask);
			const __m128i hiBytes = _mm_and_si128(mulHi, mask);
			return _mm_packus_epi16(loBytes, hiBytes);
		}

		MAG_INLINE static int8_t hsum(const native_t v) noexcept
		{
			return static_cast<int8_t>(_mm_extract_epi8(_mm_sad_epu8(v, _mm_setzero_si128()), 0));
		}

		MAG_INLINE static int8_t hmax(const native_t v) noexcept
		{
			__m128i max = v;
			max = _mm_max_epi8(max, _mm_srli_si128(max, 8));
			max = _mm_max_epi8(max, _mm_srli_si128(max, 4));
			max = _mm_max_epi8(max, _mm_srli_si128(max, 2));
			max = _mm_max_epi8(max, _mm_srli_si128(max, 1));
			return static_cast<int8_t>(_mm_extract_epi8(max, 0));
		}

		MAG_INLINE static int8_t hmin(const native_t v) noexcept
		{
			__m128i min = v;
			min = _mm_min_epi8(min, _mm_srli_si128(min, 8));
			min = _mm_min_epi8(min, _mm_srli_si128(min, 4));
			min = _mm_min_epi8(min, _mm_srli_si128(min, 2));
			min = _mm_min_epi8(min, _mm_srli_si128(min, 1));
			return static_cast<int8_t>(_mm_extract_epi8(min, 0));
		}

		MAG_INLINE static native_t max(const native_t a, const native_t b) noexcept
		{
			return _mm_max_epi8(a, b);
		}

		MAG_INLINE static native_t min(const native_t a, const native_t b) noexcept
		{
			return _mm_min_epi8(a, b);
		}
	};

	template <>
	struct ops_impl<int16_t, 8, simd_isa::sse2>
	{
		using native_t = __m128i;

		MAG_INLINE static native_t load(const int16_t* p) noexcept
		{
			return _mm_loadu_si128(reinterpret_cast<const __m128i*>(p));
		}
		MAG_INLINE static void store(int16_t* p, const native_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(p), v);
		}
		MAG_INLINE static void store(std::span<int16_t, 8> dst, const native_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(dst.data()), v);
		}

		MAG_INLINE static native_t splat(const int16_t s) noexcept { return _mm_set1_epi16(s); }

		MAG_INLINE static native_t add(const native_t a, const native_t b) noexcept
		{
			return _mm_add_epi16(a, b);
		}
		MAG_INLINE static native_t sub(const native_t a, const native_t b) noexcept
		{
			return _mm_sub_epi16(a, b);
		}
		MAG_INLINE static native_t mul(const native_t a, const native_t b) noexcept
		{
			return _mm_mullo_epi16(a, b);
		}

		MAG_INLINE static int16_t hsum(const native_t v) noexcept
		{
			return static_cast<int16_t>(_mm_extract_epi16(_mm_sad_epu8(v, _mm_setzero_si128()), 0));
		}

		MAG_INLINE static int16_t hmax(const native_t v) noexcept
		{
			__m128i max = v;
			max = _mm_max_epi16(max, _mm_srli_si128(max, 8));
			max = _mm_max_epi16(max, _mm_srli_si128(max, 4));
			max = _mm_max_epi16(max, _mm_srli_si128(max, 2));
			return static_cast<int16_t>(_mm_extract_epi16(max, 0));
		}

		MAG_INLINE static int16_t hmin(const native_t v) noexcept
		{
			__m128i min = v;
			min = _mm_min_epi16(min, _mm_srli_si128(min, 8));
			min = _mm_min_epi16(min, _mm_srli_si128(min, 4));
			min = _mm_min_epi16(min, _mm_srli_si128(min, 2));
			return static_cast<int16_t>(_mm_extract_epi16(min, 0));
		}

		MAG_INLINE static native_t max(const native_t a, const native_t b) noexcept
		{
			return _mm_max_epi16(a, b);
		}

		MAG_INLINE static native_t min(const native_t a, const native_t b) noexcept
		{
			return _mm_min_epi16(a, b);
		}
	};

	template <>
	struct ops_impl<int32_t, 4, simd_isa::sse2>
	{
		using native_t = __m128i;

		MAG_INLINE static native_t load(const int32_t* p) noexcept
		{
			return _mm_loadu_si128(reinterpret_cast<const __m128i*>(p));
		}
		MAG_INLINE static void store(int32_t* p, const native_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(p), v);
		}
		MAG_INLINE static void store(std::span<int32_t, 4> dst, const native_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(dst.data()), v);
		}

		MAG_INLINE static native_t splat(const int32_t s) noexcept { return _mm_set1_epi32(s); }

		MAG_INLINE static native_t add(const native_t a, const native_t b) noexcept
		{
			return _mm_add_epi32(a, b);
		}
		MAG_INLINE static native_t sub(const native_t a, const native_t b) noexcept
		{
			return _mm_sub_epi32(a, b);
		}
		MAG_INLINE static native_t mul(const native_t a, const native_t b) noexcept
		{
			return _mm_mullo_epi32(a, b);
		}

		MAG_INLINE static int32_t hsum(const native_t v) noexcept
		{
			return _mm_cvtsi128_si32(_mm_hadd_epi32(_mm_hadd_epi32(v, v), v));
		}

		MAG_INLINE static int32_t hmax(const native_t v) noexcept
		{
			__m128i max = v;
			max = _mm_max_epi32(max, _mm_srli_si128(max, 8));
			max = _mm_max_epi32(max, _mm_srli_si128(max, 4));
			return _mm_cvtsi128_si32(max);
		}

		MAG_INLINE static int32_t hmin(const native_t v) noexcept
		{
			__m128i min = v;
			min = _mm_min_epi32(min, _mm_srli_si128(min, 8));
			min = _mm_min_epi32(min, _mm_srli_si128(min, 4));
			return _mm_cvtsi128_si32(min);
		}

		MAG_INLINE static native_t max(const native_t a, const native_t b) noexcept
		{
			return _mm_max_epi32(a, b);
		}

		MAG_INLINE static native_t min(const native_t a, const native_t b) noexcept
		{
			return _mm_min_epi32(a, b);
		}
	};

	template <>
	struct ops_impl<int64_t, 2, simd_isa::sse2>
	{
		using native_t = __m128i;

		MAG_INLINE static native_t load(const int64_t* p) noexcept
		{
			return _mm_loadu_si128(reinterpret_cast<const __m128i*>(p));
		}
		MAG_INLINE static void store(int64_t* p, const native_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(p), v);
		}
		MAG_INLINE static void store(std::span<int64_t, 2> dst, const native_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(dst.data()), v);
		}

		MAG_INLINE static native_t splat(const int64_t s) noexcept { return _mm_set1_epi64x(s); }

		MAG_INLINE static native_t add(const native_t a, const native_t b) noexcept
		{
			return _mm_add_epi64(a, b);
		}
		MAG_INLINE static native_t sub(const native_t a, const native_t b) noexcept
		{
			return _mm_sub_epi64(a, b);
		}

		MAG_INLINE static int64_t hsum(const native_t v) noexcept
		{
			return _mm_cvtsi128_si64(_mm_add_epi64(v, _mm_srli_si128(v, 8)));
		}

		MAG_INLINE static int64_t hmax(const native_t v) noexcept
		{
			const __m128i shifted = _mm_srli_si128(v, 8);
			const __m128i mask = _mm_cmpgt_epi64(v, shifted);
			const __m128i max = _mm_blendv_epi8(shifted, v, mask);
			return _mm_cvtsi128_si64(max);
		}

		MAG_INLINE static int64_t hmin(const native_t v) noexcept
		{
			const __m128i shifted = _mm_srli_si128(v, 8);
			const __m128i mask = _mm_cmpgt_epi64(shifted, v);
			const __m128i min = _mm_blendv_epi8(v, shifted, mask);
			return _mm_cvtsi128_si64(min);
		}

		MAG_INLINE static native_t max(const native_t a, const native_t b) noexcept
		{
			return _mm_max_epi64(a, b);
		}

		MAG_INLINE static native_t min(const native_t a, const native_t b) noexcept
		{
			return _mm_min_epi64(a, b);
		}
	};

	template <>
	struct ops_impl<uint8_t, 16, simd_isa::sse2>
	{
		using native_t = __m128i;

		MAG_INLINE static native_t load(const uint8_t* p) noexcept
		{
			return _mm_loadu_si128(reinterpret_cast<const __m128i*>(p));
		}
		MAG_INLINE static void store(uint8_t* p, const native_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(p), v);
		}
		MAG_INLINE static void store(std::span<uint8_t, 16> dst, const native_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(dst.data()), v);
		}

		MAG_INLINE static native_t splat(const uint8_t s) noexcept
		{
			return _mm_set1_epi8(static_cast<char>(s));
		}

		MAG_INLINE static native_t add(const native_t a, const native_t b) noexcept
		{
			return _mm_add_epi8(a, b);
		}
		MAG_INLINE static native_t sub(const native_t a, const native_t b) noexcept
		{
			return _mm_sub_epi8(a, b);
		}
		MAG_INLINE static native_t mul(const native_t a, const native_t b) noexcept
		{
			const __m128i zeros = _mm_setzero_si128();
			const __m128i aLo = _mm_unpacklo_epi8(a, zeros);
			const __m128i aHi = _mm_unpackhi_epi8(a, zeros);
			const __m128i bLo = _mm_unpacklo_epi8(b, zeros);
			const __m128i bHi = _mm_unpackhi_epi8(b, zeros);
			const __m128i mulLo = _mm_mullo_epi16(aLo, bLo);
			const __m128i mulHi = _mm_mullo_epi16(aHi, bHi);
			const __m128i mask = _mm_set1_epi16(0x00FF);
			const __m128i loBytes = _mm_and_si128(mulLo, mask);
			const __m128i hiBytes = _mm_and_si128(mulHi, mask);
			return _mm_packus_epi16(loBytes, hiBytes);
		}

		MAG_INLINE static uint8_t hsum(const native_t v) noexcept
		{
			return static_cast<uint8_t>(_mm_extract_epi8(_mm_sad_epu8(v, _mm_setzero_si128()), 0));
		}

		MAG_INLINE static uint8_t hmax(const native_t v) noexcept
		{
			__m128i max = v;
			max = _mm_max_epi8(max, _mm_srli_si128(max, 8));
			max = _mm_max_epi8(max, _mm_srli_si128(max, 4));
			max = _mm_max_epi8(max, _mm_srli_si128(max, 2));
			max = _mm_max_epi8(max, _mm_srli_si128(max, 1));
			return static_cast<uint8_t>(_mm_extract_epi8(max, 0));
		}

		MAG_INLINE static uint8_t hmin(const native_t v) noexcept
		{
			__m128i min = v;
			min = _mm_min_epi8(min, _mm_srli_si128(min, 8));
			min = _mm_min_epi8(min, _mm_srli_si128(min, 4));
			min = _mm_min_epi8(min, _mm_srli_si128(min, 2));
			min = _mm_min_epi8(min, _mm_srli_si128(min, 1));
			return static_cast<uint8_t>(_mm_extract_epi8(min, 0));
		}

		MAG_INLINE static native_t max(const native_t a, const native_t b) noexcept
		{
			return _mm_max_epu8(a, b);
		}

		MAG_INLINE static native_t min(const native_t a, const native_t b) noexcept
		{
			return _mm_min_epu8(a, b);
		}
	};

	template <>
	struct ops_impl<uint16_t, 8, simd_isa::sse2>
	{
		using native_t = __m128i;

		MAG_INLINE static native_t load(const uint16_t* p) noexcept
		{
			return _mm_loadu_si128(reinterpret_cast<const __m128i*>(p));
		}
		MAG_INLINE static void store(uint16_t* p, const native_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(p), v);
		}
		MAG_INLINE static void store(std::span<uint16_t, 8> dst, const native_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(dst.data()), v);
		}

		MAG_INLINE static native_t splat(const uint16_t s) noexcept
		{
			return _mm_set1_epi16(static_cast<short>(s));
		}

		MAG_INLINE static native_t add(const native_t a, const native_t b) noexcept
		{
			return _mm_add_epi16(a, b);
		}
		MAG_INLINE static native_t sub(const native_t a, const native_t b) noexcept
		{
			return _mm_sub_epi16(a, b);
		}
		MAG_INLINE static native_t mul(const native_t a, const native_t b) noexcept
		{
			return _mm_mullo_epi16(a, b);
		}

		MAG_INLINE static uint16_t hsum(const native_t v) noexcept
		{
			return static_cast<uint16_t>(
					_mm_extract_epi16(_mm_sad_epu8(v, _mm_setzero_si128()), 0));
		}

		MAG_INLINE static uint16_t hmax(const native_t v) noexcept
		{
			__m128i max = v;
			max = _mm_max_epi16(max, _mm_srli_si128(max, 8));
			max = _mm_max_epi16(max, _mm_srli_si128(max, 4));
			max = _mm_max_epi16(max, _mm_srli_si128(max, 2));
			return static_cast<uint16_t>(_mm_extract_epi16(max, 0));
		}

		MAG_INLINE static uint16_t hmin(const native_t v) noexcept
		{
			__m128i min = v;
			min = _mm_min_epi16(min, _mm_srli_si128(min, 8));
			min = _mm_min_epi16(min, _mm_srli_si128(min, 4));
			min = _mm_min_epi16(min, _mm_srli_si128(min, 2));
			return static_cast<uint16_t>(_mm_extract_epi16(min, 0));
		}

		MAG_INLINE static native_t max(const native_t a, const native_t b) noexcept
		{
			return _mm_max_epu16(a, b);
		}

		MAG_INLINE static native_t min(const native_t a, const native_t b) noexcept
		{
			return _mm_min_epu16(a, b);
		}
	};

	template <>
	struct ops_impl<uint32_t, 4, simd_isa::sse2>
	{
		using native_t = __m128i;

		MAG_INLINE static native_t load(const uint32_t* p) noexcept
		{
			return _mm_loadu_si128(reinterpret_cast<const __m128i*>(p));
		}
		MAG_INLINE static void store(uint32_t* p, const native_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(p), v);
		}
		MAG_INLINE static void store(std::span<uint32_t, 4> dst, const native_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(dst.data()), v);
		}

		MAG_INLINE static native_t splat(const uint32_t s) noexcept
		{
			return _mm_set1_epi32(static_cast<int>(s));
		}

		MAG_INLINE static native_t add(const native_t a, const native_t b) noexcept
		{
			return _mm_add_epi32(a, b);
		}
		MAG_INLINE static native_t sub(const native_t a, const native_t b) noexcept
		{
			return _mm_sub_epi32(a, b);
		}
		MAG_INLINE static native_t mul(const native_t a, const native_t b) noexcept
		{
			return _mm_mullo_epi32(a, b);
		}

		MAG_INLINE static uint32_t hsum(const native_t v) noexcept
		{
			return static_cast<uint32_t>(
					_mm_cvtsi128_si32(_mm_hadd_epi32(_mm_hadd_epi32(v, v), v)));
		}

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

		MAG_INLINE static native_t max(const native_t a, const native_t b) noexcept
		{
			return _mm_max_epu32(a, b);
		}

		MAG_INLINE static native_t min(const native_t a, const native_t b) noexcept
		{
			return _mm_min_epu32(a, b);
		}
	};

	template <>
	struct ops_impl<uint64_t, 2, simd_isa::sse2>
	{
		using native_t = __m128i;

		MAG_INLINE static native_t load(const uint64_t* p) noexcept
		{
			return _mm_loadu_si128(reinterpret_cast<const __m128i*>(p));
		}
		MAG_INLINE static void store(uint64_t* p, const native_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(p), v);
		}
		MAG_INLINE static void store(std::span<uint64_t, 2> dst, const native_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(dst.data()), v);
		}

		MAG_INLINE static native_t splat(const uint64_t s) noexcept
		{
			return _mm_set1_epi64x(static_cast<long long>(s));
		}

		MAG_INLINE static native_t add(const native_t a, const native_t b) noexcept
		{
			return _mm_add_epi64(a, b);
		}
		MAG_INLINE static native_t sub(const native_t a, const native_t b) noexcept
		{
			return _mm_sub_epi64(a, b);
		}

		MAG_INLINE static uint64_t hsum(const native_t v) noexcept
		{
			return _mm_cvtsi128_si64(_mm_add_epi64(v, _mm_srli_si128(v, 8)));
		}

		MAG_INLINE static uint64_t hmax(const native_t v) noexcept
		{
			const __m128i shifted = _mm_srli_si128(v, 8);
			const __m128i max = _mm_max_epi64(v, shifted);
			return static_cast<uint64_t>(_mm_cvtsi128_si64(max));
		}

		MAG_INLINE static uint64_t hmin(const native_t v) noexcept
		{
			const __m128i shifted = _mm_srli_si128(v, 8);
			const __m128i min = _mm_min_epi64(v, shifted);
			return static_cast<uint64_t>(_mm_cvtsi128_si64(min));
		}

		MAG_INLINE static native_t max(const native_t a, const native_t b) noexcept
		{
			return _mm_max_epu64(a, b);
		}

		MAG_INLINE static native_t min(const native_t a, const native_t b) noexcept
		{
			return _mm_min_epu64(a, b);
		}
	};
} // namespace mag::simd

// NOLINTEND(portability-simd-intrinsics)
