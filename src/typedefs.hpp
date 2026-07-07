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

#pragma once

#include <concepts>

#if defined(__clang__) || defined(__GNUC__)
#define MAG_INLINE __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#define MAG_INLINE __forceinline
#else
#define MAG_INLINE inline
#endif

// Define macros to suppress compiler warnings
#if defined(__clang__) && __clang_major__ >= 22
#define MAG_DIAG_PUSH _Pragma("clang diagnostic push")
#define MAG_DIAG_POP _Pragma("clang diagnostic pop")
#define MAG_DISABLE_TU_LOCAL_ENTITY_EXPOSURE                                                       \
	_Pragma("clang diagnostic ignored \"-WTU-local-entity-exposure\"")
#else
#define MAG_DIAG_PUSH
#define MAG_DIAG_POP
#define MAG_DISABLE_TU_LOCAL_ENTITY_EXPOSURE
#endif

/**
 * @brief Verify template parameters are numeric types.
 */
template <typename T>
concept Numeric = std::is_arithmetic_v<T>;
