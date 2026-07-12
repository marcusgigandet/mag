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
#include <cstdint>
export module mag;


export import :constants;

export import :matrix;
export import :matrix_3x3;
export import :matrix_4x4;
export import :matrix_ops;

export import :vector;
export import :vector_2;
export import :vector_3;
export import :vector_4;
export import :vector_ops;

export namespace MAG_NAMESPACE
{
	using Mat2d = Mat<double, 2, 2>;
	using Mat2f = Mat<float, 2, 2>;
	using Mat3d = Mat<double, 3, 3>;
	using Mat3f = Mat<float, 3, 3>;
	using Mat4d = Mat<double, 4, 4>;
	using Mat4f = Mat<float, 4, 4>;

	using Mat2x3d = Mat<double, 2, 3>;
	using Mat2x3f = Mat<float, 2, 3>;
	using Mat3x2d = Mat<double, 3, 2>;
	using Mat3x2f = Mat<float, 3, 2>;
	using Mat3x4d = Mat<double, 3, 4>;
	using Mat3x4f = Mat<float, 3, 4>;
	using Mat4x3d = Mat<double, 4, 3>;
	using Mat4x3f = Mat<float, 4, 3>;
	using Mat2x4d = Mat<double, 2, 4>;
	using Mat2x4f = Mat<float, 2, 4>;
	using Mat4x2d = Mat<double, 4, 2>;
	using Mat4x2f = Mat<float, 4, 2>;

	using Mat2 = Mat2d;
	using Mat3 = Mat3d;
	using Mat4 = Mat4d;

	using Mat2x3 = Mat2x3d;
	using Mat3x2 = Mat3x2d;
	using Mat3x4 = Mat3x4d;
	using Mat4x3 = Mat4x3d;
	using Mat2x4 = Mat2x4d;
	using Mat4x2 = Mat4x2d;

	using Vec2d = Vec<double, 2>;
	using Vec2f = Vec<float, 2>;
	using Vec2i = Vec<int32_t, 2>;
	using Vec2u = Vec<uint32_t, 2>;
	using Vec2l = Vec<int64_t, 2>;
	using Vec2ul = Vec<uint64_t, 2>;

	using Vec3d = Vec<double, 3>;
	using Vec3f = Vec<float, 3>;
	using Vec3i = Vec<int32_t, 3>;
	using Vec3u = Vec<uint32_t, 3>;
	using Vec3l = Vec<int64_t, 3>;
	using Vec3ul = Vec<uint64_t, 3>;

	using Vec4d = Vec<double, 4>;
	using Vec4f = Vec<float, 4>;
	using Vec4i = Vec<int32_t, 4>;
	using Vec4u = Vec<uint32_t, 4>;
	using Vec4l = Vec<int64_t, 4>;
	using Vec4ul = Vec<uint64_t, 4>;

	using Vec2 = Vec2d;
	using Vec3 = Vec3d;
	using Vec4 = Vec4d;
} // namespace MAG_NAMESPACE
