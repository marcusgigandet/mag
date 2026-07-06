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
#include "typedefs.hpp"
#include <cmath>
export module mag:matrix_3x3;

import :matrix;
import :vector;

namespace mag
{
	template <Numeric T>
	struct Mat<T, 3, 3> : IMat<Mat<T, 3, 3>, T, 3, 3>
	{
		union
		{
			T m[3][3];
			struct
			{
				T m00, m01, m02;
				T m10, m11, m12;
				T m20, m21, m22;
			};
		};

		constexpr Mat() noexcept
		{
			for (auto& col : m)
				for (auto& val : col)
					val = T(0);
		}

		template <Numeric U>
		constexpr explicit Mat(U val) noexcept
		{
			for (auto& col : m)
				for (auto& v : col)
					v = static_cast<T>(val);
		}

		// clang-format off
		template <Numeric U0, Numeric U1, Numeric U2,
				  Numeric U3, Numeric U4, Numeric U5,
				  Numeric U6, Numeric U7, Numeric U8>
		constexpr Mat(U0 i00, U1 i01, U2 i02,
					  U3 i10, U4 i11, U5 i12,
					  U6 i20, U7 i21, U8 i22) noexcept :
			m00(static_cast<T>(i00)), m01(static_cast<T>(i10)), m02(static_cast<T>(i20)),
			m10(static_cast<T>(i01)), m11(static_cast<T>(i11)), m12(static_cast<T>(i21)),
			m20(static_cast<T>(i02)), m21(static_cast<T>(i12)), m22(static_cast<T>(i22))
		// clang-format on
		{
		}

		// clang-format off
		template <Numeric U>
		constexpr explicit Mat(const Mat<U, 4, 4>& o) noexcept :
			m00(static_cast<T>(o[0][0])), m01(o[0][1]), m02(o[0][2]),
			m10(static_cast<T>(o[1][0])), m11(o[1][1]), m12(o[1][2]),
			m20(static_cast<T>(o[2][0])), m21(o[2][1]), m22(o[2][2])
		// clang-format on
		{
		}

		template <Numeric U>
		constexpr Vec<T, 3> operator*(const Vec<U, 3>& v) noexcept
		{
			return {m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z,
					m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z,
					m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z};
		}

		constexpr static Mat identity() noexcept { return Mat::diagonal(1); }
		constexpr Mat inverse() const noexcept { return inverse(*this); }

		constexpr static Mat inverse(const Mat& m) noexcept
		{
			const T m00{m[0][0]}, m01{m[0][1]}, m02{m[0][2]};
			const T m10{m[1][0]}, m11{m[1][1]}, m12{m[1][2]};
			const T m20{m[2][0]}, m21{m[2][1]}, m22{m[2][2]};

			// Calculate the determinant
			const T det{
					m00 * (m11 * m22 - m21 * m12) - m10 * (m01 * m22 - m21 * m02) +
							m20 * (m01 * m12 - m11 * m02),
			};
			const T inv_det{1 / det};

			// Calculate the adjugate matrix
			// clang-format off
			return {(m11 * m22 - m21 * m12) * inv_det, (m20 * m12 - m10 * m22) * inv_det, (m10 * m21 - m20 * m11) * inv_det,
					(m21 * m02 - m01 * m22) * inv_det, (m00 * m22 - m20 * m02) * inv_det, (m20 * m01 - m00 * m21) * inv_det,
					(m01 * m12 - m11 * m02) * inv_det, (m10 * m02 - m00 * m12) * inv_det, (m00 * m11 - m10 * m01) * inv_det};
			// clang-format on
		}

		template <Numeric U>
		constexpr static Mat inverse(U x, U y, U z) noexcept
		{
			// clang-format off
			return {1 / static_cast<T>(x), 0, 0,
					0, 1 / static_cast<T>(y), 0,
					0, 0, 1 / static_cast<T>(z)};
			// clang-format on
		}

		template <Numeric U>
		constexpr static Mat rotateX(U radians) noexcept
		{
			const T c{static_cast<T>(std::cos(radians))};
			const T s{static_cast<T>(std::sin(radians))};
			// clang-format off
			return {1, 0, 0,
					0, c, -s,
					0, s, c};
			// clang-format on
		}

		template <Numeric U>
		constexpr static Mat rotateY(U radians) noexcept
		{
			const T c{static_cast<T>(std::cos(radians))};
			const T s{static_cast<T>(std::sin(radians))};
			// clang-format off
			return {c, 0, s,
					0, 1, 0,
					-s, 0, c};
			// clang-format on
		}

		template <Numeric U>
		constexpr static Mat rotateZ(U radians) noexcept
		{
			const T c{static_cast<T>(std::cos(radians))};
			const T s{static_cast<T>(std::sin(radians))};
			// clang-format off
			return {c, -s, 0,
					s, c, 0,
					0, 0, 1};
			// clang-format on
		}
	};
} // namespace mag
