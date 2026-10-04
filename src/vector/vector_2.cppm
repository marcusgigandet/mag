/*
 * SPDX-FileCopyrightText: 2026 Marcus Gigandet
 *
 * SPDX-License-Identifier: Apache-2.0
 */

module;
#include "typedefs.hpp"
export module mag:vector_2;

import :vector;

namespace MAG_NAMESPACE
{
	template <Numeric T>
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
		explicit constexpr Vec(U val) : x(val), y(val)
		{
		}

		template <Numeric U0, Numeric U1>
		constexpr Vec(U0 x, U1 y) : x(x), y(y)
		{
		}
	};
} // namespace MAG_NAMESPACE
