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
export module mag:simd_traits;

import :type_defs;

namespace mag
{
	template <Numeric T, size_t N>
	struct simd_traits
	{
		static constexpr bool supported = false; ///< Indicates if SIMD is supported for this trait.
		static constexpr bool supports_add = false; ///< Addition is not supported.
		static constexpr bool supports_sub = false; ///< Subtraction is not supported.
		static constexpr bool supports_mul = false; ///< Multiplication is not supported.
		static constexpr bool supports_div = false; ///< Division is not supported.
		using simd_t = void;						///< Placeholder for backend intrinsic type.
	};
} // namespace mag
