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
#include <cstddef>
export module mag.simd:ops;

export namespace mag::simd
{
	enum class simd_isa
	{
		sse2,
		ssse3,
		sse4_1,
		neon,
	};

	/**
	 * @brief SIMD backend operations interface.
	 *
	 * Provides the primitive SIMD operations for a given type and width.
	 *
	 * @note Inline features that may benefit from it using MAG_INLINE.
	 *
	 * @tparam T Scalar element type.
	 * @tparam N SIMD lane count.
	 * @tparam Isa SIMD instruction set architecture.
	 */
	template <Numeric T, size_t N, simd_isa Isa>
	struct ops_impl;

	// Conditionally select the default ISA based on compilation
#if defined(__AVX512F__)
#elif defined(__AVX2__)
#elif defined(__SSE4_1__)
	constexpr simd_isa default_isa = simd_isa::sse4_1;
#elif defined(__SSSE3__)
	constexpr simd_isa default_isa = simd_isa::ssse3;
#elif defined(__SSE2__)
	constexpr simd_isa default_isa = simd_isa::sse2;
#elif defined(__ARM_NEON)
	constexpr auto default_isa = simd_isa::neon;
#endif
} // namespace mag::simd
