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
export module mag:neon_ops;

import :simd_ops;
import :neon_traits;

/**
 * @brief This namespace provides SIMD operations for ARM NEON.
 *
 * It uses macros to generate repetitive code for signed/unsigned integers and floating-point types.
 */
namespace mag
{
#ifdef MAG_ENABLE_SIMD_EXTENDED

#define DEFINE_SIMD_OPS_INT(T, N, SUFFIX)                                                          \
	template <>                                                                                    \
	struct simd_ops<T, N>                                                                          \
	{                                                                                              \
		using simd_t = simd_traits<T, N>::simd_t;                                                  \
		static simd_t load(const T* p) noexcept { return vld1q_##SUFFIX(p); }                      \
		static void store(T* p, const simd_t v) noexcept { vst1q_##SUFFIX(p, v); }                 \
		static simd_t set1(const T s) noexcept { return vdupq_n_##SUFFIX(s); }                     \
		static simd_t add(const simd_t a, const simd_t b) noexcept                                 \
		{                                                                                          \
			return vaddq_##SUFFIX(a, b);                                                           \
		}                                                                                          \
		static simd_t sub(const simd_t a, const simd_t b) noexcept                                 \
		{                                                                                          \
			return vsubq_##SUFFIX(a, b);                                                           \
		}                                                                                          \
		static T horizontal_sum(const simd_t v) noexcept { return vaddvq_##SUFFIX(v); }            \
	};

	DEFINE_SIMD_OPS_INT(int8_t, 16, s8)
	DEFINE_SIMD_OPS_INT(int16_t, 8, s16)
	DEFINE_SIMD_OPS_INT(int32_t, 4, s32)
	DEFINE_SIMD_OPS_INT(uint8_t, 16, u8)
	DEFINE_SIMD_OPS_INT(uint16_t, 8, u16)
	DEFINE_SIMD_OPS_INT(uint32_t, 4, u32)

#define DEFINE_SIMD_OPS_INT_NO_MUL(T, N, SUFFIX)                                                   \
	template <>                                                                                    \
	struct simd_ops<T, N>                                                                          \
	{                                                                                              \
		using simd_t = simd_traits<T, N>::simd_t;                                                  \
		static simd_t load(const T* p) noexcept { return vld1q_##SUFFIX(p); }                      \
		static void store(T* p, const simd_t v) noexcept { vst1q_##SUFFIX(p, v); }                 \
		static simd_t set1(const T s) noexcept { return vdupq_n_##SUFFIX(s); }                     \
		static simd_t add(const simd_t a, const simd_t b) noexcept                                 \
		{                                                                                          \
			return vaddq_##SUFFIX(a, b);                                                           \
		}                                                                                          \
		static simd_t sub(const simd_t a, const simd_t b) noexcept                                 \
		{                                                                                          \
			return vsubq_##SUFFIX(a, b);                                                           \
		}                                                                                          \
		static T horizontal_sum(const simd_t v) noexcept { return vaddvq_##SUFFIX(v); }            \
	};

	DEFINE_SIMD_OPS_INT_NO_MUL(int64_t, 2, s64)
	DEFINE_SIMD_OPS_INT_NO_MUL(uint64_t, 2, u64)

#endif

#define DEFINE_SIMD_OPS_FLOAT(T, N, SUFFIX)                                                        \
	template <>                                                                                    \
	struct simd_ops<T, N>                                                                          \
	{                                                                                              \
		using simd_t = simd_traits<T, N>::simd_t;                                                  \
		static simd_t load(const T* p) noexcept { return vld1q_##SUFFIX(p); }                      \
		static void store(T* p, const simd_t v) noexcept { vst1q_##SUFFIX(p, v); }                 \
		static simd_t set1(const T s) noexcept { return vdupq_n_##SUFFIX(s); }                     \
		static simd_t add(const simd_t a, const simd_t b) noexcept                                 \
		{                                                                                          \
			return vaddq_##SUFFIX(a, b);                                                           \
		}                                                                                          \
		static simd_t sub(const simd_t a, const simd_t b) noexcept                                 \
		{                                                                                          \
			return vsubq_##SUFFIX(a, b);                                                           \
		}                                                                                          \
		static simd_t mul(const simd_t a, const simd_t b) noexcept                                 \
		{                                                                                          \
			return vmulq_##SUFFIX(a, b);                                                           \
		}                                                                                          \
		static simd_t div(const simd_t a, const simd_t b) noexcept                                 \
		{                                                                                          \
			return vdivq_##SUFFIX(a, b);                                                           \
		}                                                                                          \
		static T horizontal_sum(const simd_t v) noexcept { return vaddvq_##SUFFIX(v); }            \
	};

	DEFINE_SIMD_OPS_FLOAT(float, 4, f32);
	DEFINE_SIMD_OPS_FLOAT(double, 2, f64);
} // namespace mag
