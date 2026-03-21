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
export module mag:vector_2;

import :concepts;
import :vector;

namespace mag
{
	export template <Numeric T>
	struct alignas(8) Vec<T, 2> : IVec<Vec<T, 2>, T, 2>
	{
		union
		{
			T v[2];
			struct
			{
				T x, y;
			};
			struct
			{
				T r, g;
			};
		};

		constexpr Vec() = default;

		template <Numeric U>
		explicit constexpr Vec(U val)
		{
			for (auto& i : v)
				i = val;
		}

		template <Numeric U0, Numeric U1>
		constexpr Vec(U0 x, U1 y)
		{
			v[0] = x;
			v[1] = y;
		}
	};
} // namespace mag
