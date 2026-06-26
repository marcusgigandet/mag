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
#include <concepts>
#include <cstddef>
export module mag:simd_concepts;

import :simd_ops;

export namespace mag::simd
{
	template <typename T, size_t N>
	concept supports_splat = requires(T s) {
		{ ops<T, N>::splat(s) } -> std::same_as<typename ops<T, N>::native_t>;
	};

	template <typename T, size_t N>
	concept supports_add =
			requires(typename ops<T, N>::native_t a, typename ops<T, N>::native_t b) {
				{ ops<T, N>::add(a, b) } -> std::same_as<typename ops<T, N>::native_t>;
			};

	template <typename T, size_t N>
	concept supports_sub =
			requires(typename ops<T, N>::native_t a, typename ops<T, N>::native_t b) {
				{ ops<T, N>::sub(a, b) } -> std::same_as<typename ops<T, N>::native_t>;
			};

	template <typename T, size_t N>
	concept supports_mul =
			requires(typename ops<T, N>::native_t a, typename ops<T, N>::native_t b) {
				{ ops<T, N>::mul(a, b) } -> std::same_as<typename ops<T, N>::native_t>;
			};

	template <typename T, size_t N>
	concept supports_div =
			requires(typename ops<T, N>::native_t a, typename ops<T, N>::native_t b) {
				{ ops<T, N>::div(a, b) } -> std::same_as<typename ops<T, N>::native_t>;
			};

	template <typename T, size_t N>
	concept supports_reduction = requires(typename ops<T, N>::native_t v) {
		{ ops<T, N>::hsum(v) } -> std::same_as<T>;
	};

	template <typename T, size_t N>
	concept supports_min =
			requires(typename ops<T, N>::native_t a, typename ops<T, N>::native_t b) {
				{ ops<T, N>::min(a, b) } -> std::same_as<typename ops<T, N>::native_t>;
			};

	template <typename T, size_t N>
	concept supports_max =
			requires(typename ops<T, N>::native_t a, typename ops<T, N>::native_t b) {
				{ ops<T, N>::max(a, b) } -> std::same_as<typename ops<T, N>::native_t>;
			};

	template <typename T, size_t N>
	concept supports_neg = requires(typename ops<T, N>::native_t v) {
		{ ops<T, N>::neg(v) } -> std::same_as<typename ops<T, N>::native_t>;
	};

	template <typename T, size_t N>
	concept supports_abs = requires(typename ops<T, N>::native_t v) {
		{ ops<T, N>::abs(v) } -> std::same_as<typename ops<T, N>::native_t>;
	};

	template <typename T, size_t N>
	concept supports_sqrt = requires(typename ops<T, N>::native_t v) {
		{ ops<T, N>::sqrt(v) } -> std::same_as<typename ops<T, N>::native_t>;
	};
} // namespace mag::simd
