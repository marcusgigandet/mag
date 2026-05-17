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
import :x86_traits;

/**
 * @brief This namespace provides SIMD operations for x86 (SSE/AVX).
 */
namespace mag::simd
{
#ifdef MAG_ENABLE_SIMD_EXTENDED

#define DEFINE_SIMD_OPS_INT(T, N, LOAD, STORE, SET1, ADD, SUB, HSUM)                               \
	template <>                                                                                    \
	struct simd_ops<T, N>                                                                          \
	{                                                                                              \
		using simd_t = simd_traits<T, N>::simd_t;                                                  \
		static simd_t load(const T* p) noexcept { return LOAD(p); }                                \
		static void store(T* p, const simd_t v) noexcept { STORE(p, v); }                          \
		static simd_t splat(const T s) noexcept { return SET1(s); }                                \
		static simd_t add(const simd_t a, const simd_t b) noexcept { return ADD(a, b); }           \
		static simd_t sub(const simd_t a, const simd_t b) noexcept { return SUB(a, b); }           \
		static T horizontal_sum(const simd_t v) noexcept { return HSUM(v); }                       \
	};

	DEFINE_SIMD_OPS_INT(int8_t,
						16,
						_mm_loadu_si128,
						_mm_storeu_si128,
						_mm_set1_epi8,
						_mm_add_epi8,
						_mm_sub_epi8,
						[](__m128i v)
						{ return _mm_extract_epi8(_mm_sad_epu8(v, _mm_setzero_si128()), 0); })
	DEFINE_SIMD_OPS_INT(int16_t,
						8,
						_mm_loadu_si128,
						_mm_storeu_si128,
						_mm_set1_epi16,
						_mm_add_epi16,
						_mm_sub_epi16,
						[](__m128i v)
						{ return _mm_extract_epi16(_mm_sad_epu8(v, _mm_setzero_si128()), 0); })
	DEFINE_SIMD_OPS_INT(int32_t,
						4,
						_mm_loadu_si128,
						_mm_storeu_si128,
						_mm_set1_epi32,
						_mm_add_epi32,
						_mm_sub_epi32,
						[](__m128i v)
						{ return _mm_cvtsi128_si32(_mm_hadd_epi32(_mm_hadd_epi32(v, v), v)); })
	DEFINE_SIMD_OPS_INT(uint8_t,
						16,
						_mm_loadu_si128,
						_mm_storeu_si128,
						_mm_set1_epi8,
						_mm_add_epi8,
						_mm_sub_epi8,
						[](__m128i v)
						{ return _mm_extract_epi8(_mm_sad_epu8(v, _mm_setzero_si128()), 0); })
	DEFINE_SIMD_OPS_INT(uint16_t,
						8,
						_mm_loadu_si128,
						_mm_storeu_si128,
						_mm_set1_epi16,
						_mm_add_epi16,
						_mm_sub_epi16,
						[](__m128i v)
						{ return _mm_extract_epi16(_mm_sad_epu8(v, _mm_setzero_si128()), 0); })
	DEFINE_SIMD_OPS_INT(uint32_t,
						4,
						_mm_loadu_si128,
						_mm_storeu_si128,
						_mm_set1_epi32,
						_mm_add_epi32,
						_mm_sub_epi32,
						[](__m128i v)
						{ return _mm_cvtsi128_si32(_mm_hadd_epi32(_mm_hadd_epi32(v, v), v)); })

#define DEFINE_SIMD_OPS_INT_NO_MUL(T, N, LOAD, STORE, SET1, ADD, SUB, HSUM)                        \
	template <>                                                                                    \
	struct simd_ops<T, N>                                                                          \
	{                                                                                              \
		using simd_t = simd_traits<T, N>::simd_t;                                                  \
		static simd_t load(const T* p) noexcept { return LOAD(p); }                                \
		static void store(T* p, const simd_t v) noexcept { STORE(p, v); }                          \
		static simd_t splat(const T s) noexcept { return SET1(s); }                                \
		static simd_t add(const simd_t a, const simd_t b) noexcept { return ADD(a, b); }           \
		static simd_t sub(const simd_t a, const simd_t b) noexcept { return SUB(a, b); }           \
		static T horizontal_sum(const simd_t v) noexcept { return HSUM(v); }                       \
	};

	DEFINE_SIMD_OPS_INT_NO_MUL(int64_t,
							   2,
							   _mm_loadu_si128,
							   _mm_storeu_si128,
							   _mm_set1_epi64x,
							   _mm_add_epi64,
							   _mm_sub_epi64,
							   [](__m128i v)
							   {
								   return _mm_cvtsi128_si64(_mm_add_epi64(v, _mm_srli_si128(v, 8)));
							   })
	DEFINE_SIMD_OPS_INT_NO_MUL(uint64_t,
							   2,
							   _mm_loadu_si128,
							   _mm_storeu_si128,
							   _mm_set1_epi64x,
							   _mm_add_epi64,
							   _mm_sub_epi64,
							   [](__m128i v)
							   {
								   return _mm_cvtsi128_si64(_mm_add_epi64(v, _mm_srli_si128(v, 8)));
							   })

#endif

#define DEFINE_SIMD_OPS_FLOAT(T, N, LOAD, STORE, SET1, ADD, SUB, MUL, DIV, HSUM)                   \
	template <>                                                                                    \
	struct simd_ops<T, N>                                                                          \
	{                                                                                              \
		using simd_t = T;                                                                          \
		static simd_t load(const T* p) noexcept { return LOAD(p); }                                \
		static void store(T* p, const simd_t v) noexcept { STORE(p, v); }                          \
		static simd_t splat(const T s) noexcept { return SET1(s); }                                \
		static simd_t add(const simd_t a, const simd_t b) noexcept { return ADD(a, b); }           \
		static simd_t sub(const simd_t a, const simd_t b) noexcept { return SUB(a, b); }           \
		static simd_t mul(const simd_t a, const simd_t b) noexcept { return MUL(a, b); }           \
		static simd_t div(const simd_t a, const simd_t b) noexcept { return DIV(a, b); }           \
		static T horizontal_sum(const simd_t v) noexcept { return HSUM(v); }                       \
	};
	DEFINE_SIMD_OPS_FLOAT(float,
						  4,
						  _mm_loadu_ps,
						  _mm_storeu_ps,
						  _mm_set1_ps,
						  _mm_add_ps,
						  _mm_sub_ps,
						  _mm_mul_ps,
						  _mm_div_ps,
						  [](__m128 v) { return _mm_cvtss_f32(_mm_hadd_ps(_mm_hadd_ps(v, v), v)); })
	DEFINE_SIMD_OPS_FLOAT(double,
						  2,
						  _mm_loadu_pd,
						  _mm_storeu_pd,
						  _mm_set1_pd,
						  _mm_add_pd,
						  _mm_sub_pd,
						  _mm_mul_pd,
						  _mm_div_pd,
						  [](__m128d v) { return _mm_cvtsd_f64(_mm_hadd_pd(v, v)); })
} // namespace mag::simd
