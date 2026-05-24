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
#include <cstddef>
export module mag:simd_ops;

import :concepts;

export namespace mag
{
	/**
	 * @brief SIMD backend operations interface.
	 *
	 * Provides the primitive SIMD operations for a given type and width.
	 *
	 * Each specialization of `ops<T, N>` MUST define compatible operations in the following format:
	 * @code
	 * using native_t = <native-simd-type>;
	 * MAG_INLINE static native_t load(const T* p)
	 * MAG_INLINE static void store(T* p, const native_t v)
	 * MAG_INLINE static native_t splat(const T v)
	 * MAG_INLINE static native_t add(const native_t a, const native_t b)
	 * MAG_INLINE static native_t sub(const native_t a, const native_t b)
	 * MAG_INLINE static native_t mul(const native_t a, const native_t b)
	 * MAG_INLINE static native_t div(const native_t a, const native_t b)
	 * MAG_INLINE static T hsum(const native_t v)
	 * @endcode
	 *
	 * @note Inline features that may benefit from it using MAG_INLINE.
	 *
	 * @tparam T Scalar element type.
	 * @tparam N SIMD lane count.
	 */
	template <Numeric T, size_t N>
	struct ops;
} // namespace mag
