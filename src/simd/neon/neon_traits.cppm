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
export module mag:neon_traits;

import :simd_traits;

namespace mag
{
	template <>
	struct simd_traits<int8_t, 16>
	{
		static constexpr bool supported = true;
		static constexpr bool supports_add = true;
		static constexpr bool supports_sub = true;
		static constexpr bool supports_mul = true;
		static constexpr bool supports_div = false;
		using simd_t = int8x16_t;
	};

	template <>
	struct simd_traits<int16_t, 8>
	{
		static constexpr bool supported = true;
		static constexpr bool supports_add = true;
		static constexpr bool supports_sub = true;
		static constexpr bool supports_mul = true;
		static constexpr bool supports_div = false;
		using simd_t = int16x8_t;
	};

	template <>
	struct simd_traits<int32_t, 4>
	{
		static constexpr bool supported = true;
		static constexpr bool supports_add = true;
		static constexpr bool supports_sub = true;
		static constexpr bool supports_mul = true;
		static constexpr bool supports_div = false;
		using simd_t = int32x4_t;
	};

	template <>
	struct simd_traits<int64_t, 2>
	{
		static constexpr bool supported = true;
		static constexpr bool supports_add = true;
		static constexpr bool supports_sub = true;
		static constexpr bool supports_mul = false;
		static constexpr bool supports_div = false;
		using simd_t = int64x2_t;
	};

	template <>
	struct simd_traits<uint8_t, 16>
	{
		static constexpr bool supported = true;
		static constexpr bool supports_add = true;
		static constexpr bool supports_sub = true;
		static constexpr bool supports_mul = true;
		static constexpr bool supports_div = false;
		using simd_t = uint8x16_t;
	};

	template <>
	struct simd_traits<uint16_t, 8>
	{
		static constexpr bool supported = true;
		static constexpr bool supports_add = true;
		static constexpr bool supports_sub = true;
		static constexpr bool supports_mul = true;
		static constexpr bool supports_div = false;
		using simd_t = uint16x8_t;
	};

	template <>
	struct simd_traits<uint32_t, 4>
	{
		static constexpr bool supported = true;
		static constexpr bool supports_add = true;
		static constexpr bool supports_sub = true;
		static constexpr bool supports_mul = true;
		static constexpr bool supports_div = false;
		using simd_t = uint32x4_t;
	};

	template <>
	struct simd_traits<uint64_t, 2>
	{
		static constexpr bool supported = true;
		static constexpr bool supports_add = true;
		static constexpr bool supports_sub = true;
		static constexpr bool supports_mul = false;
		static constexpr bool supports_div = false;
		using simd_t = uint64x2_t;
	};

	template <>
	struct simd_traits<float, 4>
	{
		static constexpr bool supported = true;
		static constexpr bool supports_add = true;
		static constexpr bool supports_sub = true;
		static constexpr bool supports_mul = true;
		static constexpr bool supports_div = true;
		using simd_t = float32x4_t;
	};

	template <>
	struct simd_traits<double, 2>
	{
		static constexpr bool supported = true;
		static constexpr bool supports_add = true;
		static constexpr bool supports_sub = true;
		static constexpr bool supports_mul = true;
		static constexpr bool supports_div = true;
		using simd_t = float64x2_t;
	};
} // namespace mag
