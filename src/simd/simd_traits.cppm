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
export module mag:simd_traits;

import :concepts;

namespace mag
{
	template <typename T,
			  bool SupportsSet1,
			  bool SupportsAdd,
			  bool SupportsSub,
			  bool SupportsMul,
			  bool SupportsDiv>
	struct simd_traits_impl
	{
		/// Backend intrinsic type.
		using simd_t = T;
		/// Indicates if creating SIMD set is supported.
		static constexpr bool supports_set1 = SupportsSet1;
		/// Indicates if addition is supported.
		static constexpr bool supports_add = SupportsAdd;
		/// Indicates if subtraction is supported.
		static constexpr bool supports_sub = SupportsSub;
		/// Indicates if multiplication is not supported.
		static constexpr bool supports_mul = SupportsMul;
		/// Indicates if division is not supported.
		static constexpr bool supports_div = SupportsDiv;
	};

	template <Numeric T, size_t N>
	struct simd_traits : simd_traits_impl<void, false, false, false, false, false>
	{
	};
} // namespace mag
