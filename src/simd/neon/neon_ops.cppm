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

namespace mag
{
	template <>
	struct simd_ops<int8_t, 16>
	{
		using simd_t = simd_traits<int8_t, 16>::simd_t;

		static simd_t load(const int8_t* p) noexcept { return vld1q_s8(p); }
		static void store(int8_t* p, const simd_t v) noexcept { vst1q_s8(p, v); }

		static simd_t add(const simd_t a, const simd_t b) noexcept { return vaddq_s8(a, b); }
		static simd_t sub(const simd_t a, const simd_t b) noexcept { return vsubq_s8(a, b); }
		static simd_t mul(const simd_t a, const simd_t b) noexcept { return vmulq_s8(a, b); }
	};

	template <>
	struct simd_ops<int16_t, 8>
	{
		using simd_t = simd_traits<int16_t, 8>::simd_t;

		static simd_t load(const int16_t* p) noexcept { return vld1q_s16(p); }
		static void store(int16_t* p, const simd_t v) noexcept { vst1q_s16(p, v); }

		static simd_t add(const simd_t a, const simd_t b) noexcept { return vaddq_s16(a, b); }
		static simd_t sub(const simd_t a, const simd_t b) noexcept { return vsubq_s16(a, b); }
		static simd_t mul(const simd_t a, const simd_t b) noexcept { return vmulq_s16(a, b); }
	};

	template <>
	struct simd_ops<int32_t, 4>
	{
		using simd_t = simd_traits<int32_t, 4>::simd_t;

		static simd_t load(const int32_t* p) noexcept { return vld1q_s32(p); }
		static void store(int32_t* p, const simd_t v) noexcept { vst1q_s32(p, v); }

		static simd_t add(const simd_t a, const simd_t b) noexcept { return vaddq_s32(a, b); }
		static simd_t sub(const simd_t a, const simd_t b) noexcept { return vsubq_s32(a, b); }
		static simd_t mul(const simd_t a, const simd_t b) noexcept { return vmulq_s32(a, b); }
	};

	template <>
	struct simd_ops<int64_t, 2>
	{
		using simd_t = simd_traits<int64_t, 2>::simd_t;

		static simd_t load(const int64_t* p) noexcept { return vld1q_s64(p); }
		static void store(int64_t* p, const simd_t v) noexcept { vst1q_s64(p, v); }

		static simd_t add(const simd_t a, const simd_t b) noexcept { return vaddq_s64(a, b); }
		static simd_t sub(const simd_t a, const simd_t b) noexcept { return vsubq_s64(a, b); }
	};

	template<>
	struct simd_ops<uint8_t, 16>
	{
		using simd_t = simd_traits<uint8_t, 16>::simd_t;

		static simd_t load(const uint8_t* p) noexcept { return vld1q_u8(p); }
		static void store(uint8_t* p, const simd_t v) noexcept { vst1q_u8(p, v); }

		static simd_t add(const simd_t a, const simd_t b) noexcept { return vaddq_u8(a, b); }
		static simd_t sub(const simd_t a, const simd_t b) noexcept { return vsubq_u8(a, b); }
		static simd_t mul(const simd_t a, const simd_t b) noexcept { return vmulq_u8(a, b); }
	};

	template <>
	struct simd_ops<uint16_t, 8>
	{
		using simd_t = simd_traits<uint16_t, 8>::simd_t;

		static simd_t load(const uint16_t* p) noexcept { return vld1q_u16(p); }
		static void store(uint16_t* p, const simd_t v) noexcept { vst1q_u16(p, v); }

		static simd_t add(const simd_t a, const simd_t b) noexcept { return vaddq_u16(a, b); }
		static simd_t sub(const simd_t a, const simd_t b) noexcept { return vsubq_u16(a, b); }
		static simd_t mul(const simd_t a, const simd_t b) noexcept { return vmulq_u16(a, b); }
	};

	template <>
	struct simd_ops<uint32_t, 4>
	{
		using simd_t = simd_traits<uint32_t, 4>::simd_t;

		static simd_t load(const uint32_t* p) noexcept { return vld1q_u32(p); }
		static void store(uint32_t* p, const simd_t v) noexcept { vst1q_u32(p, v); }

		static simd_t add(const simd_t a, const simd_t b) noexcept { return vaddq_u32(a, b); }
		static simd_t sub(const simd_t a, const simd_t b) noexcept { return vsubq_u32(a, b); }
		static simd_t mul(const simd_t a, const simd_t b) noexcept { return vmulq_u32(a, b); }
	};

	template <>
	struct simd_ops<uint64_t, 2>
	{
		using simd_t = simd_traits<uint64_t, 2>::simd_t;

		static simd_t load(const uint64_t* p) noexcept { return vld1q_u64(p); }
		static void store(uint64_t* p, const simd_t v) noexcept { vst1q_u64(p, v); }

		static simd_t add(const simd_t a, const simd_t b) noexcept { return vaddq_u64(a, b); }
		static simd_t sub(const simd_t a, const simd_t b) noexcept { return vsubq_u64(a, b); }
	};

	template<>
	struct simd_ops<float, 4>
	{
		using simd_t = simd_traits<float, 4>::simd_t;

		static simd_t load(const float* p) noexcept { return vld1q_f32(p); }
		static void store(float* p, const simd_t v) noexcept { vst1q_f32(p, v); }

		static simd_t add(const simd_t a, const simd_t b) noexcept { return vaddq_f32(a, b); }
		static simd_t sub(const simd_t a, const simd_t b) noexcept { return vsubq_f32(a, b); }
		static simd_t mul(const simd_t a, const simd_t b) noexcept { return vmulq_f32(a, b); }
		static simd_t div(const simd_t a, const simd_t b) noexcept { return vdivq_f32(a, b); }
	};

	template <>
	struct simd_ops<double, 2>
	{
		using simd_t = simd_traits<double, 2>::simd_t;

		static simd_t load(const double* p) noexcept { return vld1q_f64(p); }
		static void store(double* p, const simd_t v) noexcept { vst1q_f64(p, v); }

		static simd_t add(const simd_t a, const simd_t b) noexcept { return vaddq_f64(a, b); }
		static simd_t sub(const simd_t a, const simd_t b) noexcept { return vsubq_f64(a, b); }
		static simd_t mul(const simd_t a, const simd_t b) noexcept { return vmulq_f64(a, b); }
		static simd_t div(const simd_t a, const simd_t b) noexcept { return vdivq_f64(a, b); }
	};
} // namespace mag
