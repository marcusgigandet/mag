/*
 * SPDX-FileCopyrightText: 2026 Marcus Gigandet
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <concepts>

#if defined(__clang__) || defined(__GNUC__)
#	define MAG_INLINE __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#	define MAG_INLINE __forceinline
#else
#	define MAG_INLINE inline
#endif

// Define macros to suppress compiler warnings
#if defined(__clang__) && __clang_major__ >= 22
#	define MAG_DIAG_PUSH _Pragma("clang diagnostic push")
#	define MAG_DIAG_POP _Pragma("clang diagnostic pop")
#	define MAG_DISABLE_TU_LOCAL_ENTITY_EXPOSURE                                                   \
		_Pragma("clang diagnostic ignored \"-WTU-local-entity-exposure\"")
#else
#	define MAG_DIAG_PUSH
#	define MAG_DIAG_POP
#	define MAG_DISABLE_TU_LOCAL_ENTITY_EXPOSURE
#endif

// Set the namespace to use for the project
#ifndef MAG_NAMESPACE
#	define MAG_NAMESPACE mag
#endif

namespace MAG_NAMESPACE
{
	/**
	 * @brief Verify template parameters are numeric types.
	 */
	template <typename T>
	concept Numeric = std::is_arithmetic_v<T>;
} // namespace MAG_NAMESPACE
