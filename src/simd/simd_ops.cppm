/*
 * SPDX-FileCopyrightText: 2026 Marcus Gigandet
 *
 * SPDX-License-Identifier: Apache-2.0
 */

module;
#include "typedefs.hpp"
#include <cstddef>
#include <cstdint>
export module mag.simd:ops;

export namespace MAG_NAMESPACE::simd
{
	/**
	 * @brief Enum of supported/implemented simd backends.
	 */
	enum class simd_isa : std::uint8_t
	{
		sse2,
		ssse3,
		sse4_1,
		neon, ///< Supported on Arm64 only
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
	template <Numeric T, std::size_t N, simd_isa Isa>
	struct ops_impl;

	// Conditionally select the default ISA based on the define provided to CMake
#ifdef MAG_SIMD_BACKEND_SSE4_1
	constexpr auto default_isa = simd_isa::sse4_1;
#elif defined(MAG_SIMD_BACKEND_SSSE3)
	constexpr auto default_isa = simd_isa::ssse3;
#elif defined(MAG_SIMD_BACKEND_SSE2)
	constexpr auto default_isa = simd_isa::sse2;
#elif defined(MAG_SIMD_BACKEND_NEON)
	constexpr auto default_isa = simd_isa::neon;
#endif
} // namespace MAG_NAMESPACE::simd
