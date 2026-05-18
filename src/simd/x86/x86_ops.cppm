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
#include <immintrin.h>
export module mag:x86_ops;

import :simd_ops;

/**
 * @brief This namespace provides SIMD operations for x86 (SSE/AVX).
 */
export namespace mag::simd
{
#ifdef MAG_ENABLE_SIMD_EXTENDED
	template <>
	struct ops<int8_t, 16>
	{
		using simd_t = __m128i;

		static simd_t load(const int8_t* p) noexcept
		{
			return _mm_loadu_si128(reinterpret_cast<const __m128i*>(p));
		}
		static void store(int8_t* p, const simd_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(p), v);
		}

		static simd_t splat(int8_t s) noexcept { return _mm_set1_epi8(s); }

		static simd_t add(const simd_t a, const simd_t b) noexcept { return _mm_add_epi8(a, b); }
		static simd_t sub(const simd_t a, const simd_t b) noexcept { return _mm_sub_epi8(a, b); }

		static int8_t horizontal_sum(const simd_t v) noexcept
		{
			return static_cast<int8_t>(_mm_extract_epi8(_mm_sad_epu8(v, _mm_setzero_si128()), 0));
		}
	};


	template <>
	struct ops<int16_t, 8>
	{
		using simd_t = __m128i;

		static simd_t load(const int16_t* p) noexcept
		{
			return _mm_loadu_si128(reinterpret_cast<const __m128i*>(p));
		}
		static void store(int16_t* p, const simd_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(p), v);
		}

		static simd_t splat(int16_t s) noexcept { return _mm_set1_epi16(s); }

		static simd_t add(const simd_t a, const simd_t b) noexcept { return _mm_add_epi16(a, b); }
		static simd_t sub(const simd_t a, const simd_t b) noexcept { return _mm_sub_epi16(a, b); }

		static int16_t horizontal_sum(const simd_t v) noexcept
		{
			return static_cast<int16_t>(_mm_extract_epi16(_mm_sad_epu8(v, _mm_setzero_si128()), 0));
		}
	};


	template <>
	struct ops<int32_t, 4>
	{
		using simd_t = __m128i;

		static simd_t load(const int32_t* p) noexcept
		{
			return _mm_loadu_si128(reinterpret_cast<const __m128i*>(p));
		}
		static void store(int32_t* p, const simd_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(p), v);
		}

		static simd_t splat(int32_t s) noexcept { return _mm_set1_epi32(s); }

		static simd_t add(const simd_t a, const simd_t b) noexcept { return _mm_add_epi32(a, b); }
		static simd_t sub(const simd_t a, const simd_t b) noexcept { return _mm_sub_epi32(a, b); }

		static int32_t horizontal_sum(const simd_t v) noexcept
		{
			return _mm_cvtsi128_si32(_mm_hadd_epi32(_mm_hadd_epi32(v, v), v));
		}
	};


	template <>
	struct ops<int64_t, 2>
	{
		using simd_t = __m128i;

		static simd_t load(const int64_t* p) noexcept
		{
			return _mm_loadu_si128(reinterpret_cast<const __m128i*>(p));
		}
		static void store(int64_t* p, const simd_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(p), v);
		}

		static simd_t splat(int64_t s) noexcept { return _mm_set1_epi64x(s); }

		static simd_t add(const simd_t a, const simd_t b) noexcept { return _mm_add_epi64(a, b); }
		static simd_t sub(const simd_t a, const simd_t b) noexcept { return _mm_sub_epi64(a, b); }

		static int64_t horizontal_sum(const simd_t v) noexcept
		{
			return _mm_cvtsi128_si64(_mm_add_epi64(v, _mm_srli_si128(v, 8)));
		}
	};


	template <>
	struct ops<uint8_t, 16>
	{
		using simd_t = __m128i;

		static simd_t load(const uint8_t* p) noexcept
		{
			return _mm_loadu_si128(reinterpret_cast<const __m128i*>(p));
		}
		static void store(uint8_t* p, const simd_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(p), v);
		}

		static simd_t splat(uint8_t s) noexcept { return _mm_set1_epi8(static_cast<char>(s)); }

		static simd_t add(const simd_t a, const simd_t b) noexcept { return _mm_add_epi8(a, b); }
		static simd_t sub(const simd_t a, const simd_t b) noexcept { return _mm_sub_epi8(a, b); }

		static uint8_t horizontal_sum(const simd_t v) noexcept
		{
			return static_cast<uint8_t>(_mm_extract_epi8(_mm_sad_epu8(v, _mm_setzero_si128()), 0));
		}
	};


	template <>
	struct ops<uint16_t, 8>
	{
		using simd_t = __m128i;

		static simd_t load(const uint16_t* p) noexcept
		{
			return _mm_loadu_si128(reinterpret_cast<const __m128i*>(p));
		}
		static void store(uint16_t* p, const simd_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(p), v);
		}

		static simd_t splat(uint16_t s) noexcept { return _mm_set1_epi16(static_cast<short>(s)); }

		static simd_t add(const simd_t a, const simd_t b) noexcept { return _mm_add_epi16(a, b); }
		static simd_t sub(const simd_t a, const simd_t b) noexcept { return _mm_sub_epi16(a, b); }

		static uint16_t horizontal_sum(const simd_t v) noexcept
		{
			return static_cast<uint16_t>(
					_mm_extract_epi16(_mm_sad_epu8(v, _mm_setzero_si128()), 0));
		}
	};


	template <>
	struct ops<uint32_t, 4>
	{
		using simd_t = __m128i;

		static simd_t load(const uint32_t* p) noexcept
		{
			return _mm_loadu_si128(reinterpret_cast<const __m128i*>(p));
		}
		static void store(uint32_t* p, const simd_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(p), v);
		}

		static simd_t splat(uint32_t s) noexcept { return _mm_set1_epi32(static_cast<int>(s)); }

		static simd_t add(const simd_t a, const simd_t b) noexcept { return _mm_add_epi32(a, b); }
		static simd_t sub(const simd_t a, const simd_t b) noexcept { return _mm_sub_epi32(a, b); }

		static uint32_t horizontal_sum(const simd_t v) noexcept
		{
			return static_cast<uint32_t>(
					_mm_cvtsi128_si32(_mm_hadd_epi32(_mm_hadd_epi32(v, v), v)));
		}
	};


	template <>
	struct ops<uint64_t, 2>
	{
		using simd_t = __m128i;

		static simd_t load(const uint64_t* p) noexcept
		{
			return _mm_loadu_si128(reinterpret_cast<const __m128i*>(p));
		}
		static void store(uint64_t* p, const simd_t v) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(p), v);
		}

		static simd_t splat(uint64_t s) noexcept
		{
			return _mm_set1_epi64x(static_cast<long long>(s));
		}

		static simd_t add(const simd_t a, const simd_t b) noexcept { return _mm_add_epi64(a, b); }
		static simd_t sub(const simd_t a, const simd_t b) noexcept { return _mm_sub_epi64(a, b); }

		static uint64_t horizontal_sum(const simd_t v) noexcept
		{
			return static_cast<uint64_t>(_mm_cvtsi128_si64(_mm_add_epi64(v, _mm_srli_si128(v, 8))));
		}
	};
#endif

	template <>
	struct ops<float, 4>
	{
		using simd_t = __m128;

		static simd_t load(const float* p) noexcept { return _mm_loadu_ps(p); }
		static void store(float* p, const simd_t v) noexcept { _mm_storeu_ps(p, v); }

		static simd_t splat(const float s) noexcept { return _mm_set1_ps(s); }

		static simd_t add(const simd_t a, const simd_t b) noexcept { return _mm_add_ps(a, b); }
		static simd_t sub(const simd_t a, const simd_t b) noexcept { return _mm_sub_ps(a, b); }
		static simd_t mul(const simd_t a, const simd_t b) noexcept { return _mm_mul_ps(a, b); }
		static simd_t div(const simd_t a, const simd_t b) noexcept { return _mm_div_ps(a, b); }

		static float horizontal_sum(const simd_t v) noexcept
		{
			return _mm_cvtss_f32(_mm_hadd_ps(_mm_hadd_ps(v, v), v));
		}
	};


	template <>
	struct ops<double, 2>
	{
		using simd_t = __m128d;

		static simd_t load(const double* p) noexcept { return _mm_loadu_pd(p); }
		static void store(double* p, const simd_t v) noexcept { _mm_storeu_pd(p, v); }

		static simd_t splat(const double s) noexcept { return _mm_set1_pd(s); }

		static simd_t add(const simd_t a, const simd_t b) noexcept { return _mm_add_pd(a, b); }
		static simd_t sub(const simd_t a, const simd_t b) noexcept { return _mm_sub_pd(a, b); }
		static simd_t mul(const simd_t a, const simd_t b) noexcept { return _mm_mul_pd(a, b); }
		static simd_t div(const simd_t a, const simd_t b) noexcept { return _mm_div_pd(a, b); }

		static double horizontal_sum(const simd_t v) noexcept
		{
			return _mm_cvtsd_f64(_mm_hadd_pd(v, v));
		}
	};
} // namespace mag::simd
