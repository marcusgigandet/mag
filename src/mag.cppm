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
export module mag;

export import :concepts;
export import :constants;
export import :matrix;
export import :matrix_3x3;
export import :matrix_4x4;
export import :type_defs;
export import :vector;
export import :vector_2;
export import :vector_3;
export import :vector_4;
export import :vector_ops;

#ifdef MAG_ENABLE_SIMD

#ifdef __ARM_NEON
export import :neon_ops;
export import :neon_traits;
#endif

export import :simd_ops;
export import :simd_traits;
#endif
