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
export module mag:matrix_nxn;

import :concepts;
import :matrix;

namespace mag
{
	export template <Numeric T, size_t N>
	constexpr Mat<T, N, N> identity() noexcept
	{
		Mat<T, N, N> result{};
		for (size_t i = 0; i < N; ++i)
			result[i][i] = static_cast<T>(1);
		return result;
	}
} // namespace mag
