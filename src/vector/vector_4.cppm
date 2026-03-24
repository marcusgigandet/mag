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
export module mag:vector_4;

import :concepts;
import :vector;

namespace mag
{
	export template <Numeric T>
	struct alignas(16) Vec<T, 4> : IVec<Vec<T, 4>, T, 4>
	{
		union
		{
			T v[4];
			struct
			{
				T x, y, z, w;
			};
			struct
			{
				T r, g, b, a;
			};
		};

		constexpr Vec() = default;

		template <Numeric U>
		explicit constexpr Vec(U val)
		{
			x = val;
			y = val;
			z = val;
			w = val;
		}

		template <Numeric U0, Numeric U1, Numeric U2, Numeric U3>
		constexpr Vec(U0 x, U1 y, U2 z, U3 w)
		{
			v[0] = x;
			v[1] = y;
			v[2] = z;
			v[3] = w;
		}
	};
} // namespace mag
