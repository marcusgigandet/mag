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
export module mag:x86_ops;

import :simd_ops;

MAG_DIAG_PUSH
MAG_DISABLE_TU_LOCAL_ENTITY_EXPOSURE

/**
 * @brief This namespace provides SIMD operations for x86 (SSE/AVX).
 */
namespace mag
{
#ifdef MAG_ENABLE_SIMD_EXTENDED
	template <>
	struct ops<int8_t, 16>
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

		MAG_INLINE static native_t splat(int8_t s) noexcept { return _mm_set1_epi8(s); }

		MAG_INLINE static native_t add(const native_t a, const native_t b) noexcept
		{
			return _mm_add_epi8(a, b);
		}
		MAG_INLINE static native_t sub(const native_t a, const native_t b) noexcept
		{
			return _mm_sub_epi8(a, b);
		}

		MAG_INLINE static int8_t hsum(const native_t v) noexcept
		{
			return static_cast<int8_t>(_mm_extract_epi8(_mm_sad_epu8(v, _mm_setzero_si128()), 0));
		}
	};


	template <>
	struct ops<int16_t, 8>
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

		MAG_INLINE static native_t splat(int16_t s) noexcept { return _mm_set1_epi16(s); }

		MAG_INLINE static native_t add(const native_t a, const native_t b) noexcept
		{
			return _mm_add_epi16(a, b);
		}
		MAG_INLINE static native_t sub(const native_t a, const native_t b) noexcept
		{
			return _mm_sub_epi16(a, b);
		}

		MAG_INLINE static int16_t hsum(const native_t v) noexcept
		{
			return static_cast<int16_t>(_mm_extract_epi16(_mm_sad_epu8(v, _mm_setzero_si128()), 0));
		}
	};


	template <>
	struct ops<int32_t, 4>
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

		MAG_INLINE static native_t splat(int32_t s) noexcept { return _mm_set1_epi32(s); }

		MAG_INLINE static native_t add(const native_t a, const native_t b) noexcept
		{
			return _mm_add_epi32(a, b);
		}
		MAG_INLINE static native_t sub(const native_t a, const native_t b) noexcept
		{
			return _mm_sub_epi32(a, b);
		}

		MAG_INLINE static int32_t hsum(const native_t v) noexcept
		{
			return _mm_cvtsi128_si32(_mm_hadd_epi32(_mm_hadd_epi32(v, v), v));
		}
	};


	template <>
	struct ops<int64_t, 2>
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

		MAG_INLINE static native_t splat(int64_t s) noexcept { return _mm_set1_epi64x(s); }

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
	};


	template <>
	struct ops<uint8_t, 16>
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

		MAG_INLINE static native_t splat(uint8_t s) noexcept
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

		MAG_INLINE static uint8_t hsum(const native_t v) noexcept
		{
			return static_cast<uint8_t>(_mm_extract_epi8(_mm_sad_epu8(v, _mm_setzero_si128()), 0));
		}
	};


	template <>
	struct ops<uint16_t, 8>
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

		MAG_INLINE static native_t splat(uint16_t s) noexcept
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

		MAG_INLINE static uint16_t hsum(const native_t v) noexcept
		{
			return static_cast<uint16_t>(
					_mm_extract_epi16(_mm_sad_epu8(v, _mm_setzero_si128()), 0));
		}
	};


	template <>
	struct ops<uint32_t, 4>
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

		MAG_INLINE static native_t splat(uint32_t s) noexcept
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

		MAG_INLINE static uint32_t hsum(const native_t v) noexcept
		{
			return static_cast<uint32_t>(
					_mm_cvtsi128_si32(_mm_hadd_epi32(_mm_hadd_epi32(v, v), v)));
		}
	};


	template <>
	struct ops<uint64_t, 2>
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

		MAG_INLINE static native_t splat(uint64_t s) noexcept
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
			return static_cast<uint64_t>(_mm_cvtsi128_si64(_mm_add_epi64(v, _mm_srli_si128(v, 8))));
		}
	};
#endif

	template <>
	struct ops<float, 4>
	{
		using native_t = __m128;

		MAG_INLINE static native_t load(const float* p) noexcept { return _mm_loadu_ps(p); }
		MAG_INLINE static void store(float* p, const native_t v) noexcept { _mm_storeu_ps(p, v); }

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

		MAG_INLINE static float hsum(const native_t v) noexcept
		{
			return _mm_cvtss_f32(_mm_hadd_ps(_mm_hadd_ps(v, v), v));
		}
	};


	template <>
	struct ops<double, 2>
	{
		using native_t = __m128d;

		MAG_INLINE static native_t load(const double* p) noexcept { return _mm_loadu_pd(p); }
		MAG_INLINE static void store(double* p, const native_t v) noexcept { _mm_storeu_pd(p, v); }

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

		MAG_INLINE static double hsum(const native_t v) noexcept
		{
			return _mm_cvtsd_f64(_mm_hadd_pd(v, v));
		}
	};
} // namespace mag

MAG_DIAG_POP
