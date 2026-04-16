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
import :simd_traits;

namespace mag
{
	template <typename T, size_t N>
	struct simd_ops
	{
		using simd_t = simd_traits<T, N>::simd_t;

		static simd_t load(const T* p) noexcept = delete;
		static void store(T* p, simd_t v) noexcept = delete;
		static simd_t set1(T s) noexcept = delete;
		static simd_t add(simd_t a, simd_t b) noexcept = delete;
		static simd_t sub(simd_t a, simd_t b) noexcept = delete;
		static simd_t mul(simd_t a, simd_t b) noexcept = delete;
		static simd_t div(simd_t a, simd_t b) noexcept = delete;
		static T horizontal_sum(simd_t v) noexcept = delete;
	};
} // namespace mag
