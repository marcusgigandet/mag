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
#include <cmath>
export module mag:matrix_4x4;

import :concepts;
import :constants;
import :matrix;
import :vector_3;
import :vector_4;

namespace mag
{
	export template <typename T>
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
					  U12 i30, U13 i31, U14 i32, U15 i33) noexcept
		:
			m00(static_cast<T>(i00)), m01(static_cast<T>(i10)), m02(static_cast<T>(i20)), m03(static_cast<T>(i30)),
			m10(static_cast<T>(i01)), m11(static_cast<T>(i11)), m12(static_cast<T>(i21)), m13(static_cast<T>(i31)),
			m20(static_cast<T>(i02)), m21(static_cast<T>(i12)), m22(static_cast<T>(i22)), m23(static_cast<T>(i32)),
			m30(static_cast<T>(i03)), m31(static_cast<T>(i13)), m32(static_cast<T>(i23)), m33(static_cast<T>(i33))
		{
		}
		// clang-format on

		template <Numeric U>
		constexpr Mat(const Vec<U, 4>& col0,
					  const Vec<U, 4>& col1,
					  const Vec<U, 4>& col2,
					  const Vec<U, 4>& col3) noexcept
		{
			m[0][0] = static_cast<T>(col0.x);
			m[0][1] = static_cast<T>(col0.y);
			m[0][2] = static_cast<T>(col0.z);
			m[0][3] = static_cast<T>(col0.w);

			m[1][0] = static_cast<T>(col1.x);
			m[1][1] = static_cast<T>(col1.y);
			m[1][2] = static_cast<T>(col1.z);
			m[1][3] = static_cast<T>(col1.w);

			m[2][0] = static_cast<T>(col2.x);
			m[2][1] = static_cast<T>(col2.y);
			m[2][2] = static_cast<T>(col2.z);
			m[2][3] = static_cast<T>(col2.w);

			m[3][0] = static_cast<T>(col3.x);
			m[3][1] = static_cast<T>(col3.y);
			m[3][2] = static_cast<T>(col3.z);
			m[3][3] = static_cast<T>(col3.w);
		}

		template <Numeric U>
		constexpr Vec<T, 4> operator*(const Vec<U, 4>& v) const noexcept
		{
			return {m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * v.w,
					m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * v.w,
					m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * v.w,
					m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3] * v.w};
		}

		constexpr Mat inverse() const noexcept
		{
			return inverse(*this);
		}

		constexpr static Mat inverse(const Mat& m) noexcept
		{
			const T a = m[0][0], b = m[0][1], c = m[0][2], d = m[0][3];
			const T e = m[1][0], f = m[1][1], g = m[1][2], h = m[1][3];
			const T i = m[2][0], j = m[2][1], k = m[2][2], l = m[2][3];
			const T m0 = m[3][0], n = m[3][1], o = m[3][2], p = m[3][3];

			// Calculate the 4x4 determinant
			const T det = a * (f * (k * p - l * o) - g * (j * p - l * n) + h * (j * o - k * n)) -
						  b * (e * (k * p - l * o) - g * (i * p - l * m0) + h * (i * o - k * m0)) +
						  c * (e * (j * p - l * n) - f * (i * p - l * m0) + h * (i * n - j * m0)) -
						  d * (e * (j * o - k * n) - f * (i * o - k * m0) + g * (i * n - j * m0));

			const T inv_det = static_cast<T>(1) / det;

			// Calculate the adjugate matrix
			// clang-format off
			return {
				(f * (k * p - l * o) - g * (j * p - l * n) + h * (j * o - k * n)) * inv_det,
				(e * (k * p - l * o) - g * (i * p - l * m0) + h * (i * o - k * m0)) * -inv_det,
				(e * (j * p - l * n) - f * (i * p - l * m0) + h * (i * n - j * m0)) * inv_det,
				(e * (j * o - k * n) - f * (i * o - k * m0) + g * (i * n - j * m0)) * -inv_det,

				(b * (k * p - l * o) - c * (j * p - l * n) + d * (j * o - k * n)) * -inv_det,
				(a * (k * p - l * o) - c * (i * p - l * m0) + d * (i * o - k * m0)) * inv_det,
				(a * (j * p - l * n) - b * (i * p - l * m0) + d * (i * n - j * m0)) * -inv_det,
				(a * (j * o - k * n) - b * (i * o - k * m0) + c * (i * n - j * m0)) * inv_det,

				(b * (g * p - h * o) - c * (f * p - h * n) + d * (f * o - g * n)) * inv_det,
				(a * (g * p - h * o) - c * (e * p - h * m0) + d * (e * o - g * m0)) * -inv_det,
				(a * (f * p - h * n) - b * (e * p - h * m0) + d * (e * n - f * m0)) * inv_det,
				(a * (f * o - g * n) - b * (e * o - g * m0) + c * (e * n - f * m0)) * -inv_det,

				(b * (g * l - h * k) - c * (f * l - h * j) + d * (f * k - g * j)) * -inv_det,
				(a * (g * l - h * k) - c * (e * l - h * i) + d * (e * k - g * i)) * inv_det,
				(a * (f * l - h * j) - b * (e * l - h * i) + d * (e * j - f * i)) * -inv_det,
				(a * (f * k - g * j) - b * (e * k - g * i) + c * (e * j - f * i)) * inv_det
			};
			// clang-format on
		}


		template <Numeric U>
		constexpr static Mat inverse(U x, U y, U z) noexcept
		{
			// clang-format off
			return {static_cast<T>(1) / static_cast<T>(x), 0, 0, 0,
					0, static_cast<T>(1) / static_cast<T>(y), 0, 0,
					0, 0, static_cast<T>(1) / static_cast<T>(z), 0,
					0, 0, 0, static_cast<T>(1)};
			// clang-format on
		}

		constexpr static Mat identity() noexcept
		{
			return Mat::diagonal();
		}

		template <Numeric U>
		constexpr static Mat translate(U x, U y, U z) noexcept
		{
			// clang-format off
			return {Vec<T, 4>{1, 0, 0, 0},
					Vec<T, 4>{0, 1, 0, 0},
					Vec<T, 4>{0, 0, 1, 0},
					Vec<T, 4>{static_cast<T>(x), static_cast<T>(y), static_cast<T>(z), 1}};
			// clang-format on
		}

		template <Numeric U>
		constexpr static Mat translate(const Vec<U, 3>& v) noexcept
		{
			return translate(v.x, v.y, v.z);
		}

		template <Numeric U>
		constexpr static Mat scale(U x, U y, U z) noexcept
		{
			// clang-format off
			return {static_cast<T>(x), 0, 0, 0,
					0, static_cast<T>(y), 0, 0,
					0, 0, static_cast<T>(z), 0,
					0, 0, 0, 1};
			// clang-format on
		}

		template <Numeric U>
		constexpr static Mat scale(const Vec<U, 3>& v) noexcept
		{
			return scale(v.x, v.y, v.z);
		}

		template <Numeric U>
		constexpr static Mat rotateX(U radians) noexcept
		{
			const T c = static_cast<T>(std::cos(radians));
			const T s = static_cast<T>(std::sin(radians));
			// clang-format off
			return {1, 0, 0, 0,
					0, c, -s, 0,
					0, s, c, 0,
					0, 0, 0, 1};
			// clang-format on
		}

		template <Numeric U>
		constexpr static Mat rotateY(U radians) noexcept
		{
			const T c = static_cast<T>(std::cos(radians));
			const T s = static_cast<T>(std::sin(radians));
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
			const T c = static_cast<T>(std::cos(radians));
			const T s = static_cast<T>(std::sin(radians));
			// clang-format off
			return {c, -s, 0, 0,
					s, c, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1};
			// clang-format on
		}

		template <Numeric U>
		constexpr static Mat
		lookAt(const Vec<U, 3>& eye, const Vec<U, 3>& center, const Vec<U, 3>& up) noexcept
		{
			const Vec<U, 3> f = (center - eye).normalized(); // Forward vector
			const Vec<U, 3> s = f.cross(up).normalized();	 // Right vector
			const Vec<U, 3> u = s.cross(f);

			// clang-format off
			return Mat{s.x, s.y, s.z, -s.dot(eye),
					   u.x, u.y, u.z, -u.dot(eye),
					   -f.x, -f.y, -f.z, f.dot(eye),
					   0, 0, 0, 1};
			// clang-format on
		}

		template <Numeric U>
		constexpr static Mat perspective(U fovY_degrees, U aspect, U nearZ, U farZ) noexcept
		{
			const U fovY = fovY_degrees * DEG_TO_RAD;
			const U f = 1.0 / tan(fovY * 0.5);
			const U A = farZ / (nearZ - farZ);
			const U B = (farZ * nearZ) / (nearZ - farZ);

			// clang-format off
			return {f / aspect, 0,  0,  0,
					0,          f,  0,  0,
					0,          0,  A,  B,
					0,          0, -1,  0};
			// clang-format on
		}
	};
} // namespace mag
