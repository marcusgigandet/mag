// clang-format off

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

import mag;
using namespace mag;

TEST_CASE("Mat4x4 construction and access", "[Mat4x4]")
{
	SECTION("Default constructor")
	{
		for (Mat4f m ; const auto v : m)
			REQUIRE(v == Catch::Approx(0.0f));
	}

	SECTION("Scalar constructor")
	{
		for (Mat4f m{1.0f} ; const auto v : m)
			REQUIRE(v == Catch::Approx(1.0f));
	}

	SECTION("Element-wise constructor")
	{
		Mat4f m(1, 2, 3, 4,
				5, 6, 7, 8,
				9, 10, 11, 12,
				13, 14, 15, 16);
		REQUIRE(m[0][0] == 1.0f);
		REQUIRE(m[1][0] == 2.0f);
		REQUIRE(m[2][0] == 3.0f);
		REQUIRE(m[3][0] == 4.0f);
		REQUIRE(m[0][1] == 5.0f);
		REQUIRE(m[1][1] == 6.0f);
		REQUIRE(m[3][3] == 16.0f);
	}

	SECTION("Named element access")
	{
		Mat4f m(1, 2, 3, 4,
				5, 6, 7, 8,
				9, 10, 11, 12,
				13, 14, 15, 16);
		REQUIRE(m.m00 == 1.0f);
		REQUIRE(m.m10 == 2.0f);
		REQUIRE(m.m33 == 16.0f);
	}

	SECTION("data() method")
	{
		Mat4f m(1, 2, 3, 4,
				5, 6, 7, 8,
				9, 10, 11, 12,
				13, 14, 15, 16);
		const float* data = m.data();
		REQUIRE(data[0] == 1.0f);
		REQUIRE(data[1] == 5.0f);
		REQUIRE(data[15] == 16.0f);
	}
}

TEST_CASE("Mat4x4 arithmetic operations", "[Mat4x4]")
{
	Mat4f a(1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16);
	Mat4f b(16, 15, 14, 13,
			12, 11, 10, 9,
			8, 7, 6, 5,
			4, 3, 2, 1);

	SECTION("Addition")
	{
		auto c = a + b;
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				REQUIRE(c[i][j] == Catch::Approx(17.0f));
	}

	SECTION("Subtraction")
	{
		auto c = a - b;
		Mat4f expected{-15, -13, -11, -9,
					   -7, -5, -3, -1,
					   1, 3, 5, 7,
					   9, 11, 13, 15};

		REQUIRE(c == expected);
	}

	SECTION("Scalar multiplication")
	{
		auto c = a * 2.0f;
		REQUIRE(c[0][0] == Catch::Approx(2.0f));
		REQUIRE(c[1][1] == Catch::Approx(12.0f));
		REQUIRE(c[3][3] == Catch::Approx(32.0f));
	}

	SECTION("Scalar division")
	{
		auto c = b / 2.0f;
		REQUIRE(c[0][0] == Catch::Approx(8.0f));
		REQUIRE(c[1][1] == Catch::Approx(5.5f));
		REQUIRE(c[3][3] == Catch::Approx(0.5f));
	}

	SECTION("Matrix multiplication")
	{
		Mat4f identity = Mat4f::identity();
		auto c = a * identity;
		REQUIRE(c == a);
	}

	SECTION("Vector multiplication")
	{
		Vec<float, 4> expected{30.0f, 70.0f, 110.0f, 150.0f};
		Vec<float, 4> v(1, 2, 3, 4);
		auto r = a * v;
		REQUIRE(r == expected);
	}

	SECTION("Compound assignment")
	{
		Mat4f c = a;
		c += b;
		REQUIRE(c == Mat4f(17.0f));

		c -= a;
		REQUIRE(c == b);

		c *= 2.0f;
		REQUIRE(c[0][0] == Catch::Approx(32.0f));
		REQUIRE(c[3][3] == Catch::Approx(2.0f));

		c /= 2.0f;
		REQUIRE(c == b);
	}
}

TEST_CASE("Mat4x4 transformations", "[Mat4x4]")
{
	SECTION("Identity")
	{
		Mat4f identity = Mat4f::identity();
		REQUIRE(identity[0][0] == 1.0f);
		REQUIRE(identity[1][1] == 1.0f);
		REQUIRE(identity[2][2] == 1.0f);
		REQUIRE(identity[3][3] == 1.0f);
	}

	SECTION("Transpose")
	{
		Mat4f m(1, 2, 3, 4,
				5, 6, 7, 8,
				9, 10, 11, 12,
				13, 14, 15, 16);

		auto t = m.transpose();
		REQUIRE(t[0][0] == 1.0f);
		REQUIRE(t[0][1] == 2.0f);
		REQUIRE(t[1][0] == 5.0f);
		REQUIRE(t[3][3] == 16.0f);
	}

	SECTION("Translation")
	{
		auto t = Mat4f::translate(1.0f, 2.0f, 3.0f);
		REQUIRE(t[3][0] == 1.0f);
		REQUIRE(t[3][1] == 2.0f);
		REQUIRE(t[3][2] == 3.0f);
		REQUIRE(t[3][3] == 1.0f);

		Vec<float, 4> v(1, 1, 1, 1);
		auto r = t * v;
		REQUIRE(r.x == Catch::Approx(2.0f));
		REQUIRE(r.y == Catch::Approx(3.0f));
		REQUIRE(r.z == Catch::Approx(4.0f));
		REQUIRE(r.w == Catch::Approx(1.0f));
	}

	SECTION("Scaling")
	{
		auto s = Mat4f::diagonal({2.0f, 3.0f, 4.0f, 1.0f});
		REQUIRE(s[0][0] == 2.0f);
		REQUIRE(s[1][1] == 3.0f);
		REQUIRE(s[2][2] == 4.0f);
		REQUIRE(s[3][3] == 1.0f);

		Vec<float, 4> v(1, 1, 1, 1);
		auto r = s * v;
		REQUIRE(r.x == Catch::Approx(2.0f));
		REQUIRE(r.y == Catch::Approx(3.0f));
		REQUIRE(r.z == Catch::Approx(4.0f));
		REQUIRE(r.w == Catch::Approx(1.0f));
	}

	SECTION("Rotation")
	{
		// Test X rotation
		auto rx = Mat4f::rotateX(pi<float> / 2.0f); // 90 degrees
		Vec<float, 4> v(0, 1, 0, 1);
		auto r = rx * v;
		REQUIRE(r.x == Catch::Approx(0.0f).margin(1e-5f));
		REQUIRE(r.y == Catch::Approx(0.0f).margin(1e-5f));
		REQUIRE(r.z == Catch::Approx(1.0f).margin(1e-5f));
		REQUIRE(r.w == Catch::Approx(1.0f).margin(1e-5f));

		// Test Y rotation
		auto ry = Mat4f::rotateY(pi<float> / 2.0f); // 90 degrees
		v = Vec<float, 4>(1, 0, 0, 1);
		r = ry * v;
		REQUIRE(r.x == Catch::Approx(0.0f).margin(1e-5f));
		REQUIRE(r.y == Catch::Approx(0.0f).margin(1e-5f));
		REQUIRE(r.z == Catch::Approx(-1.0f).margin(1e-5f));
		REQUIRE(r.w == Catch::Approx(1.0f).margin(1e-5f));

		// Test Z rotation
		auto rz = Mat4f::rotateZ(pi<float> / 2.0f); // 90 degrees
		v = Vec<float, 4>(1, 0, 0, 1);
		r = rz * v;
		REQUIRE(r.x == Catch::Approx(0.0f).margin(1e-5f));
		REQUIRE(r.y == Catch::Approx(1.0f).margin(1e-5f));
		REQUIRE(r.z == Catch::Approx(0.0f).margin(1e-5f));
		REQUIRE(r.w == Catch::Approx(1.0f).margin(1e-5f));
	}

	SECTION("Perspective projection")
	{
		auto p = Mat4f::perspective(70.0f, 1.0f, 0.1f, 100.0f);
		REQUIRE(p[0][0] == Catch::Approx(1.428148031f));
		REQUIRE(p[1][1] == Catch::Approx(1.428148031f));
		REQUIRE(p[2][2] == Catch::Approx(-1.001001f));
		REQUIRE(p[2][3] == Catch::Approx(-1.0f));
		REQUIRE(p[3][2] == Catch::Approx(-0.1001001f));
		REQUIRE(p[3][3] == Catch::Approx(0.0f));
	}

	SECTION("LookAt")
	{
		// Camera at (0,0,1) looking at origin.
		Vec<float, 3> eye(0, 0, 1);
		Vec<float, 3> center(0, 0, 0);
		Vec<float, 3> up(0, 1, 0);

		auto view{Mat4f::lookAt(eye, center, up)};
		Mat4f expected{1, 0, 0, 0,
					   0, 1, 0, 0,
					   0, 0, 1, -1,
					   0, 0, 0, 1};
		REQUIRE(view == expected);
	}
}

TEST_CASE("Mat4x4 comparison", "[Mat4x4]")
{
	Mat4f a(1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16);
	Mat4f b(1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16);
	Mat4f c(16, 15, 14, 13,
			12, 11, 10, 9,
			8, 7, 6, 5,
			4, 3, 2, 1);

	REQUIRE(a == b);
	REQUIRE(a != c);
}

TEST_CASE("Mat4x4 scalar, diagonal, and iterator utilities", "[Mat4x4]")
{
	Mat4f a{1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16};
	Mat4f b{2, 3, 4, 5,
			6, 7, 8, 9,
			10, 11, 12, 13,
			14, 15, 16, 17};

	SECTION("Scalar add/subtract free and compound operators")
	{
		auto add = a + 2.0f;
		auto sub = a - 1.0f;
		REQUIRE(add[0][0] == Catch::Approx(3.0f));
		REQUIRE(add[3][3] == Catch::Approx(18.0f));
		REQUIRE(sub[0][0] == Catch::Approx(0.0f));
		REQUIRE(sub[3][3] == Catch::Approx(15.0f));

		Mat4f c = a;
		c += 2.0f;
		c -= 2.0f;
		REQUIRE(c == a);
	}

	SECTION("Element-wise matrix multiplication compound")
	{
		Mat4f c = a;
		c *= b;
		REQUIRE(c[0][0] == Catch::Approx(2.0f));
		REQUIRE(c[1][1] == Catch::Approx(42.0f));
		REQUIRE(c[3][3] == Catch::Approx(272.0f));
	}

	SECTION("Diagonal vector helper")
	{
		auto d = Mat4f::diagonal(Vec<float, 4>{2.0f, 3.0f, 4.0f, 5.0f});
		REQUIRE(d[0][0] == Catch::Approx(2.0f));
		REQUIRE(d[1][1] == Catch::Approx(3.0f));
		REQUIRE(d[2][2] == Catch::Approx(4.0f));
		REQUIRE(d[3][3] == Catch::Approx(5.0f));
	}
}

TEST_CASE("Mat4x4 inverse and transform overloads", "[Mat4x4]")
{
	SECTION("Member inverse matches static inverse")
	{
		Mat4f m{1, 2, 3, 4,
				0, 1, 4, 2,
				5, 6, 0, 1,
				0, 0, 0, 1};
		REQUIRE(m.inverse() == Mat4f::inverse(m));
	}

	SECTION("Inverse of transform matrix composes to identity")
	{
		auto t = Mat4f::translate(2.0f, -3.0f, 4.0f);
		auto r = Mat4f::rotateZ(pi<float> / 4.0f);
		auto m = t * r;
		auto inv = Mat4f::inverse(m);
		auto product = m * inv;
		for (int c = 0; c < 4; ++c)
		{
			for (int rIdx = 0; rIdx < 4; ++rIdx)
			{
				REQUIRE(product[c][rIdx] == Catch::Approx(c == rIdx ? 1.0f : 0.0f).margin(1e-4f));
			}
		}
	}

	SECTION("Translation vector overload")
	{
		Vec<float, 3> delta{1.0f, 2.0f, 3.0f};
		auto t = Mat4f::translate(delta);
		Vec<float, 4> v{1.0f, 1.0f, 1.0f, 1.0f};
		auto r = t * v;
		REQUIRE(r.x == Catch::Approx(2.0f));
		REQUIRE(r.y == Catch::Approx(3.0f));
		REQUIRE(r.z == Catch::Approx(4.0f));
		REQUIRE(r.w == Catch::Approx(1.0f));
	}
}
