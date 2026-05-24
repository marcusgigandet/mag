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
#include <arm_neon.h>
#include <cstdint>
export module mag:neon_ops;

import :simd_ops;

// Suppress "TU local entity ___ is exposed" errors on some compilers. This warning can be ignored
// since inlining the functions calling native simd functions result in the exposure.
MAG_DIAG_PUSH
MAG_DISABLE_TU_LOCAL_ENTITY_EXPOSURE

namespace mag
{
#ifdef MAG_ENABLE_SIMD_EXTENDED
	template <>
	struct ops<int8_t, 16>
	{
		using native_t = int8x16_t;

		MAG_INLINE static native_t load(const int8_t* p) noexcept { return vld1q_s8(p); }
		MAG_INLINE static void store(int8_t* p, const native_t v) noexcept { vst1q_s8(p, v); }

		MAG_INLINE static native_t splat(const int8_t s) noexcept { return vdupq_n_s8(s); }

		MAG_INLINE static native_t add(const native_t a, const native_t b) noexcept
		{
			return vaddq_s8(a, b);
		}
		MAG_INLINE static native_t sub(const native_t a, const native_t b) noexcept
		{
			return vsubq_s8(a, b);
		}
		MAG_INLINE static native_t mul(const native_t a, const native_t b) noexcept
		{
			return vmulq_s8(a, b);
		}

		MAG_INLINE static int8_t hsum(native_t v) noexcept { return vaddvq_s8(v); }
	};


	template <>
	struct ops<int16_t, 8>
	{
		using native_t = int16x8_t;

		MAG_INLINE static native_t load(const int16_t* p) noexcept { return vld1q_s16(p); }
		MAG_INLINE static void store(int16_t* p, const native_t v) noexcept { vst1q_s16(p, v); }

		MAG_INLINE static native_t splat(const int16_t s) noexcept { return vdupq_n_s16(s); }

		MAG_INLINE static native_t add(const native_t a, const native_t b) noexcept
		{
			return vaddq_s16(a, b);
		}
		MAG_INLINE static native_t sub(const native_t a, const native_t b) noexcept
		{
			return vsubq_s16(a, b);
		}
		MAG_INLINE static native_t mul(const native_t a, const native_t b) noexcept
		{
			return vmulq_s16(a, b);
		}

		MAG_INLINE static int16_t hsum(const native_t v) noexcept { return vaddvq_s16(v); }
	};


	template <>
	struct ops<int32_t, 4>
	{
		using native_t = int32x4_t;

		MAG_INLINE static native_t load(const int32_t* p) noexcept { return vld1q_s32(p); }
		MAG_INLINE static void store(int32_t* p, const native_t v) noexcept { vst1q_s32(p, v); }

		MAG_INLINE static native_t splat(const int32_t s) noexcept { return vdupq_n_s32(s); }

		MAG_INLINE static native_t add(const native_t a, const native_t b) noexcept
		{
			return vaddq_s32(a, b);
		}
		MAG_INLINE static native_t sub(const native_t a, const native_t b) noexcept
		{
			return vsubq_s32(a, b);
		}
		MAG_INLINE static native_t mul(const native_t a, const native_t b) noexcept
		{
			return vmulq_s32(a, b);
		}

		MAG_INLINE static int32_t hsum(const native_t v) noexcept { return vaddvq_s32(v); }
	};


	template <>
	struct ops<int64_t, 2>
	{
		using native_t = int64x2_t;

		MAG_INLINE static native_t load(const int64_t* p) noexcept { return vld1q_s64(p); }
		MAG_INLINE static void store(int64_t* p, const native_t v) noexcept { vst1q_s64(p, v); }

		MAG_INLINE static native_t splat(const int64_t s) noexcept { return vdupq_n_s64(s); }

		MAG_INLINE static native_t add(const native_t a, const native_t b) noexcept
		{
			return vaddq_s64(a, b);
		}
		MAG_INLINE static native_t sub(const native_t a, const native_t b) noexcept
		{
			return vsubq_s64(a, b);
		}

		MAG_INLINE static int64_t hsum(const native_t v) noexcept { return vaddvq_s64(v); }
	};


	template <>
	struct ops<uint8_t, 16>
	{
		using native_t = uint8x16_t;

		MAG_INLINE static native_t load(const uint8_t* p) noexcept { return vld1q_u8(p); }
		MAG_INLINE static void store(uint8_t* p, const native_t v) noexcept { vst1q_u8(p, v); }

		MAG_INLINE static native_t splat(const uint8_t s) noexcept { return vdupq_n_u8(s); }

		MAG_INLINE static native_t add(const native_t a, const native_t b) noexcept
		{
			return vaddq_u8(a, b);
		}
		MAG_INLINE static native_t sub(const native_t a, const native_t b) noexcept
		{
			return vsubq_u8(a, b);
		}
		MAG_INLINE static native_t mul(const native_t a, const native_t b) noexcept
		{
			return vmulq_u8(a, b);
		}

		MAG_INLINE static uint8_t hsum(const native_t v) noexcept { return vaddvq_u8(v); }
	};


	template <>
	struct ops<uint16_t, 8>
	{
		using native_t = uint16x8_t;

		MAG_INLINE static native_t load(const uint16_t* p) noexcept { return vld1q_u16(p); }
		MAG_INLINE static void store(uint16_t* p, const native_t v) noexcept { vst1q_u16(p, v); }

		MAG_INLINE static native_t splat(const uint16_t s) noexcept { return vdupq_n_u16(s); }

		MAG_INLINE static native_t add(const native_t a, const native_t b) noexcept
		{
			return vaddq_u16(a, b);
		}
		MAG_INLINE static native_t sub(const native_t a, const native_t b) noexcept
		{
			return vsubq_u16(a, b);
		}
		MAG_INLINE static native_t mul(const native_t a, const native_t b) noexcept
		{
			return vmulq_u16(a, b);
		}

		MAG_INLINE static uint16_t hsum(const native_t v) noexcept { return vaddvq_u16(v); }
	};


	template <>
	struct ops<uint32_t, 4>
	{
		using native_t = uint32x4_t;

		MAG_INLINE static native_t load(const uint32_t* p) noexcept { return vld1q_u32(p); }
		MAG_INLINE static void store(uint32_t* p, const native_t v) noexcept { vst1q_u32(p, v); }

		MAG_INLINE static native_t splat(uint32_t s) noexcept { return vdupq_n_u32(s); }

		MAG_INLINE static native_t add(const native_t a, const native_t b) noexcept
		{
			return vaddq_u32(a, b);
		}
		MAG_INLINE static native_t sub(const native_t a, const native_t b) noexcept
		{
			return vsubq_u32(a, b);
		}
		MAG_INLINE static native_t mul(const native_t a, const native_t b) noexcept
		{
			return vmulq_u32(a, b);
		}

		MAG_INLINE static uint32_t hsum(const native_t v) noexcept { return vaddvq_u32(v); }
	};


	template <>
	struct ops<uint64_t, 2>
	{
		using native_t = uint64x2_t;

		MAG_INLINE static native_t load(const uint64_t* p) noexcept { return vld1q_u64(p); }
		MAG_INLINE static void store(uint64_t* p, const native_t v) noexcept { vst1q_u64(p, v); }

		MAG_INLINE static native_t splat(const uint64_t s) noexcept { return vdupq_n_u64(s); }

		MAG_INLINE static native_t add(const native_t a, const native_t b) noexcept
		{
			return vaddq_u64(a, b);
		}
		MAG_INLINE static native_t sub(const native_t a, const native_t b) noexcept
		{
			return vsubq_u64(a, b);
		}

		MAG_INLINE static uint64_t hsum(const native_t v) noexcept { return vaddvq_u64(v); }
	};
#endif

	template <>
	struct ops<float, 4>
	{
		using native_t = float32x4_t;

		MAG_INLINE static native_t load(const float* p) noexcept { return vld1q_f32(p); }
		MAG_INLINE static void store(float* p, const native_t v) noexcept { vst1q_f32(p, v); }

		MAG_INLINE static native_t splat(const float s) noexcept { return vdupq_n_f32(s); }

		MAG_INLINE static native_t add(const native_t a, const native_t b) noexcept
		{
			return vaddq_f32(a, b);
		}
		MAG_INLINE static native_t sub(const native_t a, const native_t b) noexcept
		{
			return vsubq_f32(a, b);
		}
		MAG_INLINE static native_t mul(const native_t a, const native_t b) noexcept
		{
			return vmulq_f32(a, b);
		}
		MAG_INLINE static native_t div(const native_t a, const native_t b) noexcept
		{
			return vdivq_f32(a, b);
		}

		MAG_INLINE static float hsum(const native_t v) noexcept { return vaddvq_f32(v); }
	};

	template <>
	struct ops<double, 2>
	{
		using native_t = float64x2_t;

		MAG_INLINE static native_t load(const double* p) noexcept { return vld1q_f64(p); }
		MAG_INLINE static void store(double* p, const native_t v) noexcept { vst1q_f64(p, v); }

		MAG_INLINE static native_t splat(const double s) noexcept { return vdupq_n_f64(s); }


		MAG_INLINE static native_t add(const native_t a, const native_t b) noexcept
		{
			return vaddq_f64(a, b);
		}
		MAG_INLINE static native_t sub(const native_t a, const native_t b) noexcept
		{
			return vsubq_f64(a, b);
		}
		MAG_INLINE static native_t mul(const native_t a, const native_t b) noexcept
		{
			return vmulq_f64(a, b);
		}
		MAG_INLINE static native_t div(const native_t a, const native_t b) noexcept
		{
			return vdivq_f64(a, b);
		}

		MAG_INLINE static double hsum(const native_t v) noexcept { return vaddvq_f64(v); }
	};
} // namespace mag

// Restore to default state
MAG_DIAG_POP
