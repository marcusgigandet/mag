/*
 * SPDX-FileCopyrightText: 2026 Marcus Gigandet
 *
 * SPDX-License-Identifier: Apache-2.0
 */

module;
#include "typedefs.hpp"
#include <cstdint>
export module mag.simd;

export import :simd;
export import :abi;
export import :concepts;
export import :ops;

#ifdef __AVX512F__
#	warning Unsupported SIMD ISA
#endif
#ifdef __AVX2__
#	warning Unsupported SIMD ISA
#endif

#ifdef MAG_SIMD_BACKEND_SSE4_1
export import :sse4_1;
#elif defined(MAG_SIMD_BACKEND_SSSE3)
export import :ssse3;
#elif defined(MAG_SIMD_BACKEND_SSE2)
export import :sse2;
#elif defined(MAG_SIMD_BACKEND_NEON)
export import :neon;
#else
#	error No supported SIMD ISA was provided!
#endif

export namespace MAG_NAMESPACE::simd
{
	using f32x4 = Simd<float, 4>;
	using f64x2 = Simd<double, 2>;
	using f32x	= native_simd<float>;
	using f64x	= native_simd<double>;

	using i8x16 = Simd<std::int8_t, 16>;
	using i16x8 = Simd<std::int16_t, 8>;
	using i32x4 = Simd<std::int32_t, 4>;
	using i64x2 = Simd<std::int64_t, 2>;
	using i8x	= native_simd<std::int8_t>;
	using i16x	= native_simd<std::int16_t>;
	using i32x	= native_simd<std::int32_t>;
	using i64x	= native_simd<std::int64_t>;

	using u8x16 = Simd<std::uint8_t, 16>;
	using u16x8 = Simd<std::uint16_t, 8>;
	using u32x4 = Simd<std::uint32_t, 4>;
	using u64x2 = Simd<std::uint64_t, 2>;
	using u8x	= native_simd<std::uint8_t>;
	using u16x	= native_simd<std::uint16_t>;
	using u32x	= native_simd<std::uint32_t>;
	using u64x	= native_simd<std::uint64_t>;
} // namespace MAG_NAMESPACE::simd
