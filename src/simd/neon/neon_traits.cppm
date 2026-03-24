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
		static constexpr bool supported = true;		///< SIMD is supported for int8_t, size 16.
		static constexpr bool supports_add = true;	///< Addition is supported.
		static constexpr bool supports_sub = true;	///< Subtraction is supported.
		static constexpr bool supports_mul = true;	///< Multiplication is supported.
		static constexpr bool supports_div = false; ///< Division is not supported.
		using simd_t = int8x16_t;					///< NEON intrinsic type for 16x8-bit integers.
	};

	template <>
	struct simd_traits<int16_t, 8>
	{
		static constexpr bool supported = true;		///< SIMD is supported for int16_t, size 8.
		static constexpr bool supports_add = true;	///< Addition is supported.
		static constexpr bool supports_sub = true;	///< Subtraction is supported.
		static constexpr bool supports_mul = true;	///< Multiplication is supported.
		static constexpr bool supports_div = false; ///< Division is not supported.
		using simd_t = int16x8_t;					///< NEON intrinsic type for 8x16-bit integers.
	};

	template <>
	struct simd_traits<int32_t, 4>
	{
		static constexpr bool supported = true;		///< SIMD is supported for int32_t, size 4.
		static constexpr bool supports_add = true;	///< Addition is supported.
		static constexpr bool supports_sub = true;	///< Subtraction is supported.
		static constexpr bool supports_mul = true;	///< Multiplication is supported.
		static constexpr bool supports_div = false; ///< Division is not supported.
		using simd_t = int32x4_t;					///< NEON intrinsic type for 4x32-bit integers.
	};

	template <>
	struct simd_traits<int64_t, 2>
	{
		static constexpr bool supported = true;		///< SIMD is supported for int32_t, size 4.
		static constexpr bool supports_add = true;	///< Addition is supported.
		static constexpr bool supports_sub = true;	///< Subtraction is supported.
		static constexpr bool supports_mul = false; ///< Multiplication is not supported.
		static constexpr bool supports_div = false; ///< Division is not supported.
		using simd_t = int64x2_t;					///< NEON intrinsic type for 2x64-bit integers.
	};

	template <>
	struct simd_traits<uint8_t, 16>
	{
		static constexpr bool supported = true;		///< SIMD is supported for uint8_t, size 16.
		static constexpr bool supports_add = true;	///< Addition is supported.
		static constexpr bool supports_sub = true;	///< Subtraction is supported.
		static constexpr bool supports_mul = true;	///< Multiplication is supported.
		static constexpr bool supports_div = false; ///< Division is not supported.
		using simd_t = uint8x16_t; ///< NEON intrinsic type for 16x8-bit unsigned integers.
	};

	template <>
	struct simd_traits<uint16_t, 8>
	{
		static constexpr bool supported = true;		///< SIMD is supported for uint16_t, size 8.
		static constexpr bool supports_add = true;	///< Addition is supported.
		static constexpr bool supports_sub = true;	///< Subtraction is supported.
		static constexpr bool supports_mul = true;	///< Multiplication is supported.
		static constexpr bool supports_div = false; ///< Division is not supported.
		using simd_t = uint16x8_t; ///< NEON intrinsic type for 8x16-bit unsigned integers.
	};

	template <>
	struct simd_traits<uint32_t, 4>
	{
		static constexpr bool supported = true;		///< SIMD is supported for uint32_t, size 4.
		static constexpr bool supports_add = true;	///< Addition is supported.
		static constexpr bool supports_sub = true;	///< Subtraction is supported.
		static constexpr bool supports_mul = true;	///< Multiplication is supported.
		static constexpr bool supports_div = false; ///< Division is not supported.
		using simd_t = uint32x4_t; ///< NEON intrinsic type for 4x32-bit unsigned integers.
	};

	template <>
	struct simd_traits<uint64_t, 2>
	{
		static constexpr bool supported = true;		///< SIMD is supported for uint32_t, size 4.
		static constexpr bool supports_add = true;	///< Addition is supported.
		static constexpr bool supports_sub = true;	///< Subtraction is supported.
		static constexpr bool supports_mul = false; ///< Multiplication is not supported.
		static constexpr bool supports_div = false; ///< Division is not supported.
		using simd_t = uint64x2_t; ///< NEON intrinsic type for 2x64-bit unsigned integers.
	};

	template <>
	struct simd_traits<float, 4>
	{
		static constexpr bool supported = true;	   ///< SIMD is supported for float, size 4.
		static constexpr bool supports_add = true; ///< Addition is supported.
		static constexpr bool supports_sub = true; ///< Subtraction is supported.
		static constexpr bool supports_mul = true; ///< Multiplication is supported.
		static constexpr bool supports_div = true; ///< Division is supported.
		using simd_t = float32x4_t;				   ///< NEON intrinsic type for 4x32-bit floats.
	};

	template <>
	struct simd_traits<double, 2>
	{
		static constexpr bool supported = true;	   ///< SIMD is supported for double, size 2.
		static constexpr bool supports_add = true; ///< Addition is supported.
		static constexpr bool supports_sub = true; ///< Subtraction is supported.
		static constexpr bool supports_mul = true; ///< Multiplication is supported.
		static constexpr bool supports_div = true; ///< Division is supported.
		using simd_t = float64x2_t;				   ///< NEON intrinsic type for 2x64-bit doubles.
	};
} // namespace mag
