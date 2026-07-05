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
#include <cstdint>
export module mag.simd;

export import :simd;
export import :abi;
export import :concepts;
export import :ops;

#ifdef __AVX512F__
#warning Unsupported SIMD ISA
#endif
#ifdef __AVX2__
#warning Unsupported SIMD ISA
#endif
#ifdef __SSE2__
export import :sse2;
#endif
#ifdef __SSSE3__
export import :ssse3;
#endif
#ifdef __SSE4_1__
export import :sse4_1;
#endif
#ifdef __ARM_NEON
export import :neon;
#endif

export namespace mag::simd
{
	using f32x4 = Simd<float, 4>;
	using f64x2 = Simd<double, 2>;
	using f32x = native_simd<float>;
	using f64x = native_simd<double>;

	using i8x16 = Simd<int8_t, 16>;
	using i16x8 = Simd<int16_t, 8>;
	using i32x4 = Simd<int32_t, 4>;
	using i64x2 = Simd<int64_t, 2>;
	using i8x = native_simd<int8_t>;
	using i16x = native_simd<int16_t>;
	using i32x = native_simd<int32_t>;
	using i64x = native_simd<int64_t>;

	using u8x16 = Simd<uint8_t, 16>;
	using u16x8 = Simd<uint16_t, 8>;
	using u32x4 = Simd<uint32_t, 4>;
	using u64x2 = Simd<uint64_t, 2>;
	using u8x = native_simd<uint8_t>;
	using u16x = native_simd<uint16_t>;
	using u32x = native_simd<uint32_t>;
	using u64x = native_simd<uint64_t>;
} // namespace mag::simd
