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
export module mag:matrix_4x4;

import :constants;
import :matrix;
import :vector;

namespace MAG_NAMESPACE
{
	template <Numeric T>
	struct Mat<T, 4, 4> : IMat<Mat<T, 4, 4>, T, 4, 4>
	{
		union
		{
			T m[4][4];
			struct
			{
				T m00, m01, m02, m03;
				T m10, m11, m12, m13;
				T m20, m21, m22, m23;
				T m30, m31, m32, m33;
			};
		};

		constexpr Mat() noexcept
		{
			for (auto& col : m)
				for (auto& v : col)
					v = static_cast<T>(0);
		}

		template <Numeric U>
		constexpr explicit Mat(U val) noexcept
		{
			for (auto& col : m)
				for (auto& v : col)
					v = static_cast<T>(val);
		}

		// clang-format off
		template <Numeric U0, Numeric U1, Numeric U2, Numeric U3,
				  Numeric U4, Numeric U5, Numeric U6, Numeric U7,
				  Numeric U8, Numeric U9, Numeric U10, Numeric U11,
				  Numeric U12, Numeric U13, Numeric U14, Numeric U15>
		constexpr Mat(U0 i00, U1 i01, U2 i02, U3 i03,
					  U4 i10, U5 i11, U6 i12, U7 i13,
					  U8 i20, U9 i21, U10 i22, U11 i23,
					  U12 i30, U13 i31, U14 i32, U15 i33) noexcept :
			m00(static_cast<T>(i00)), m01(static_cast<T>(i10)), m02(static_cast<T>(i20)), m03(static_cast<T>(i30)),
			m10(static_cast<T>(i01)), m11(static_cast<T>(i11)), m12(static_cast<T>(i21)), m13(static_cast<T>(i31)),
			m20(static_cast<T>(i02)), m21(static_cast<T>(i12)), m22(static_cast<T>(i22)), m23(static_cast<T>(i32)),
			m30(static_cast<T>(i03)), m31(static_cast<T>(i13)), m32(static_cast<T>(i23)), m33(static_cast<T>(i33))
		{
		}
		// clang-format on

		// clang-format off
		template <Numeric U>
		constexpr Mat(const Vec<U, 4>& col0,
					  const Vec<U, 4>& col1,
					  const Vec<U, 4>& col2,
					  const Vec<U, 4>& col3) noexcept :
			m00(static_cast<T>(col0.x)), m01(static_cast<T>(col0.y)), m02(static_cast<T>(col0.z)), m03(static_cast<T>(col0.w)),
			m10(static_cast<T>(col1.x)), m11(static_cast<T>(col1.y)), m12(static_cast<T>(col1.z)), m13(static_cast<T>(col1.w)),
			m20(static_cast<T>(col2.x)), m21(static_cast<T>(col2.y)), m22(static_cast<T>(col2.z)), m23(static_cast<T>(col2.w)),
			m30(static_cast<T>(col3.x)), m31(static_cast<T>(col3.y)), m32(static_cast<T>(col3.z)), m33(static_cast<T>(col3.w))
		// clang-format on
		{
		}

		template <Numeric U>
		constexpr Vec<T, 4> operator*(const Vec<U, 4>& v) const noexcept
		{
			return {m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * v.w,
					m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * v.w,
					m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * v.w,
					m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3] * v.w};
		}

		constexpr static Mat identity() noexcept { return Mat::diagonal(1); }

		constexpr Mat inverse() const noexcept { return inverse(*this); }

		constexpr static Mat inverse(const Mat& mat) noexcept
		{
			const T m00{mat[0][0]}, m01{mat[0][1]}, m02{mat[0][2]}, m03{mat[0][3]};
			const T m10{mat[1][0]}, m11{mat[1][1]}, m12{mat[1][2]}, m13{mat[1][3]};
			const T m20{mat[2][0]}, m21{mat[2][1]}, m22{mat[2][2]}, m23{mat[2][3]};
			const T m30{mat[3][0]}, m31{mat[3][1]}, m32{mat[3][2]}, m33{mat[3][3]};

			// Calculate the 4x4 determinant
			// clang-format off
			const T det{
				m00 * (m11 * (m22 * m33 - m23 * m32) - m12 * (m21 * m33 - m23 * m31)
					+ m13 * (m21 * m32 - m22 * m31)) -
				m01 * (m10 * (m22 * m33 - m23 * m32) - m12 * (m20 * m33 - m23 * m30)
					+ m13 * (m20 * m32 - m22 * m30)) +
				m02 * (m10 * (m21 * m33 - m23 * m31) - m11 * (m20 * m33 - m23 * m30)
					+ m13 * (m20 * m31 - m21 * m30)) -
				m03 * (m10 * (m21 * m32 - m22 * m31) - m11 * (m20 * m32 - m22 * m30)
					+ m12 * (m20 * m31 - m21 * m30))};
			// clang-format on

			const T inv_det{1 / det};

			// Calculate the adjugate matrix
			// clang-format off
			return {
				(m11 * (m22 * m33 - m23 * m32) - m12 * (m21 * m33 - m23 * m31) + m13 * (m21 * m32 - m22 * m31)) * inv_det,
				(m10 * (m22 * m33 - m23 * m32) - m12 * (m20 * m33 - m23 * m30) + m13 * (m20 * m32 - m22 * m30)) * -inv_det,
				(m10 * (m21 * m33 - m23 * m31) - m11 * (m20 * m33 - m23 * m30) + m13 * (m20 * m31 - m21 * m30)) * inv_det,
				(m10 * (m21 * m32 - m22 * m31) - m11 * (m20 * m32 - m22 * m30) + m12 * (m20 * m31 - m21 * m30)) * -inv_det,

				(m01 * (m22 * m33 - m23 * m32) - m02 * (m21 * m33 - m23 * m31) + m03 * (m21 * m32 - m22 * m31)) * -inv_det,
				(m00 * (m22 * m33 - m23 * m32) - m02 * (m20 * m33 - m23 * m30) + m03 * (m20 * m32 - m22 * m30)) * inv_det,
				(m00 * (m21 * m33 - m23 * m31) - m01 * (m20 * m33 - m23 * m30) + m03 * (m20 * m31 - m21 * m30)) * -inv_det,
				(m00 * (m21 * m32 - m22 * m31) - m01 * (m20 * m32 - m22 * m30) + m02 * (m20 * m31 - m21 * m30)) * inv_det,

				(m01 * (m12 * m33 - m13 * m32) - m02 * (m11 * m33 - m13 * m31) + m03 * (m11 * m32 - m12 * m31)) * inv_det,
				(m00 * (m12 * m33 - m13 * m32) - m02 * (m10 * m33 - m13 * m30) + m03 * (m10 * m32 - m12 * m30)) * -inv_det,
				(m00 * (m11 * m33 - m13 * m31) - m01 * (m10 * m33 - m13 * m30) + m03 * (m10 * m31 - m11 * m30)) * inv_det,
				(m00 * (m11 * m32 - m12 * m31) - m01 * (m10 * m32 - m12 * m30) + m02 * (m10 * m31 - m11 * m30)) * -inv_det,

				(m01 * (m12 * m23 - m13 * m22) - m02 * (m11 * m23 - m13 * m21) + m03 * (m11 * m22 - m12 * m21)) * -inv_det,
				(m00 * (m12 * m23 - m13 * m22) - m02 * (m10 * m23 - m13 * m20) + m03 * (m10 * m22 - m12 * m20)) * inv_det,
				(m00 * (m11 * m23 - m13 * m21) - m01 * (m10 * m23 - m13 * m20) + m03 * (m10 * m21 - m11 * m20)) * -inv_det,
				(m00 * (m11 * m22 - m12 * m21) - m01 * (m10 * m22 - m12 * m20) + m02 * (m10 * m21 - m11 * m20)) * inv_det
			};
			// clang-format on
		}

		template <Numeric U>
		constexpr static Mat inverse(U x, U y, U z) noexcept
		{
			// clang-format off
			return {1 / static_cast<T>(x), 0, 0, 0,
					0, 1 / static_cast<T>(y), 0, 0,
					0, 0, 1 / static_cast<T>(z), 0,
					0, 0, 0, 1};
			// clang-format on
		}

		template <Numeric U>
		constexpr static Mat translate(U x, U y, U z) noexcept
		{
			// clang-format off
			return {1, 0, 0, static_cast<T>(x),
					0, 1, 0, static_cast<T>(y),
					0, 0, 1, static_cast<T>(z),
					0, 0, 0, 1};
			// clang-format on
		}

		template <Numeric U>
		constexpr static Mat translate(const Vec<U, 3>& v) noexcept
		{
			return translate(v.x, v.y, v.z);
		}

		template <Numeric U>
		constexpr static Mat rotateX(U radians) noexcept
		{
			const T c{static_cast<T>(std::cos(radians))};
			const T s{static_cast<T>(std::sin(radians))};
			// clang-format off
			return {1, 0,  0, 0,
					0, c, -s, 0,
					0, s,  c, 0,
					0, 0,  0, 1};
			// clang-format on
		}

		template <Numeric U>
		constexpr static Mat rotateY(U radians) noexcept
		{
			const T c{static_cast<T>(std::cos(radians))};
			const T s{static_cast<T>(std::sin(radians))};
			// clang-format off
			return {c, 0, s, 0,
					0, 1, 0, 0,
				   -s, 0, c, 0,
					0, 0, 0, 1};
			// clang-format on
		}

		template <Numeric U>
		constexpr static Mat rotateZ(U radians) noexcept
		{
			const T c{static_cast<T>(std::cos(radians))};
			const T s{static_cast<T>(std::sin(radians))};
			// clang-format off
			return {c, -s, 0, 0,
					s,  c, 0, 0,
					0,  0, 1, 0,
					0,  0, 0, 1};
			// clang-format on
		}

		template <Numeric U>
		constexpr static Mat
		lookAt(const Vec<U, 3>& eye, const Vec<U, 3>& center, const Vec<U, 3>& up) noexcept
		{
			const Vec<U, 3> f{(center - eye).normalized()}; // Forward vector
			const Vec<U, 3> s{cross(f, up).normalized()};	// Right vector
			const Vec<U, 3> u{cross(s, f)};

			// clang-format off
			return Mat{s.x,  s.y,  s.z, -s.dot(eye),
					   u.x,  u.y,  u.z, -u.dot(eye),
				      -f.x, -f.y, -f.z,  f.dot(eye),
					   0,    0,    0,    1};
			// clang-format on
		}

		template <Numeric U>
		constexpr static Mat perspective(U fovY_degrees, U aspect, U nearZ, U farZ) noexcept
		{
			const U fovY{static_cast<U>(fovY_degrees * deg_to_rad<U>)};
			const U f{static_cast<U>(1.0 / tan(fovY * 0.5))};
			const U A{farZ / (nearZ - farZ)};
			const U B{(farZ * nearZ) / (nearZ - farZ)};

			// clang-format off
			return {f / aspect, 0,  0,  0,
					0,          f,  0,  0,
					0,          0,  A,  B,
					0,          0, -1,  0};
			// clang-format on
		}
	};
} // namespace MAG_NAMESPACE
