/*
 * SPDX-FileCopyrightText: 2026 Marcus Gigandet
 *
 * SPDX-License-Identifier: Apache-2.0
 */

module;
#include "typedefs.hpp"
#include <type_traits>
export module mag:vector_3;

import :vector;

namespace MAG_NAMESPACE
{
	export template <Numeric T, Numeric U>
	constexpr auto cross(const Vec<T, 3>& a, const Vec<U, 3>& b) noexcept
	{
		using R = std::common_type_t<T, U>;
		return Vec<R, 3>{
			a[1] * b[2] - a[2] * b[1],
			a[2] * b[0] - a[0] * b[2],
			a[0] * b[1] - a[1] * b[0],
		};
	}


	template <Numeric T>
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

		constexpr Vec() = default;

		template <Numeric U>
		explicit constexpr Vec(U val) : x(val), y(val), z(val)
		{
		}

		template <Numeric U0, Numeric U1, Numeric U2>
		constexpr Vec(U0 x, U1 y, U2 z) : x(x), y(y), z(z)
		{
		}

		template <Numeric U>
		constexpr auto cross(const Vec<U, 3>& other) const noexcept
		{
			return MAG_NAMESPACE::cross(*this, other);
		}
	};
} // namespace MAG_NAMESPACE
