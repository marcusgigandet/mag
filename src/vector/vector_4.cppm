/*
 * SPDX-FileCopyrightText: 2026 Marcus Gigandet
 *
 * SPDX-License-Identifier: Apache-2.0
 */

module;
#include "typedefs.hpp"
export module mag:vector_4;

import :vector;

namespace MAG_NAMESPACE
{
	template <Numeric T>
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
		explicit constexpr Vec(U val) : x(val), y(val), z(val), w(val)
		{
		}

		template <Numeric U0, Numeric U1, Numeric U2, Numeric U3>
		constexpr Vec(U0 x, U1 y, U2 z, U3 w) : x(x), y(y), z(z), w(w)
		{
		}
	};
} // namespace MAG_NAMESPACE
