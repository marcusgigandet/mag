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
#include <type_traits>
export module mag:vector_3;

import :concepts;
import :vector;

namespace mag
{
	export template <Numeric T>
	struct alignas(16) Vec<T, 3> : IVec<Vec<T, 3>, T, 3>
	{
		union
		{
			T v[3];
			struct
			{
				T x, y, z;
			};
			struct
			{
				T r, g, b;
			};
		};

	private:
		T _pad{}; ///< Unused var, specified for 16-byte alignment

	public:
		constexpr Vec() = default;

		template <Numeric U>
		explicit constexpr Vec(U val)
		{
			for (auto& i : v)
				i = val;
		}

		template <Numeric U0, Numeric U1, Numeric U2>
		constexpr Vec(U0 x, U1 y, U2 z)
		{
			v[0] = x;
			v[1] = y;
			v[2] = z;
		}

		template <Numeric U>
		constexpr Vec<std::common_type_t<T, U>, 3> cross(const Vec<U, 3>& o) const noexcept
		{
			using R = std::common_type_t<T, U>;
			return Vec<R, 3>{
					v[1] * o[2] - v[2] * o[1],
					v[2] * o[0] - v[0] * o[2],
					v[0] * o[1] - v[1] * o[0],
			};
		}
	};

	export template <Numeric T, Numeric U>
	constexpr auto cross(const Vec<T, 3>& a, const Vec<U, 3>& b) noexcept
	{
		return a.cross(b);
	}
} // namespace mag
