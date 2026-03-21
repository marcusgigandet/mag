// clang-format off

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

import mag;
using namespace mag;

TEST_CASE("Mat3x3 construction and access", "[Mat3x3]")
{
	SECTION("Default constructor")
	{
		Mat3f m;
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				REQUIRE(m[i][j] == 0.0f);
	}

	SECTION("Scalar constructor")
	{
		Mat3f m(1.0f);
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				REQUIRE(m[i][j] == 1.0f);
	}

	SECTION("Element-wise constructor")
	{
		Mat3f m(1, 2, 3,
				4, 5, 6,
				7, 8, 9);
		REQUIRE(m[0][0] == 1.0f);
		REQUIRE(m[0][1] == 4.0f);
		REQUIRE(m[0][2] == 7.0f);
		REQUIRE(m[1][0] == 2.0f);
		REQUIRE(m[1][1] == 5.0f);
		REQUIRE(m[1][2] == 8.0f);
		REQUIRE(m[2][0] == 3.0f);
		REQUIRE(m[2][1] == 6.0f);
		REQUIRE(m[2][2] == 9.0f);
	}

	SECTION("Named element access")
	{
		Mat3f m(1, 2, 3,
				4, 5, 6,
				7, 8, 9);
		REQUIRE(m.m00 == 1.0f);
		REQUIRE(m.m10 == 2.0f);
		REQUIRE(m.m22 == 9.0f);
	}

	SECTION("data() method")
	{
		Mat3f m(1, 2, 3,
				4, 5, 6,
				7, 8, 9);
		const float* data = m.data();
		REQUIRE(data[0] == 1.0f);
		REQUIRE(data[4] == 5.0f);
		REQUIRE(data[8] == 9.0f);
	}
}

TEST_CASE("Mat3x3 arithmetic operations", "[Mat3x3]")
{
	Mat3f a(1, 2, 3,
			4, 5, 6,
			7, 8, 9);
	Mat3f b(16, 15, 14,
			13, 12, 11,
			10, 9, 8);

	SECTION("Addition")
	{
		auto c = a + b;
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				REQUIRE(c[i][j] == Catch::Approx(17.0f));
	}

	SECTION("Subtraction")
	{
		auto c = a - b;
		REQUIRE(c == Mat3f{-15, -13, -11,
						   -9, -7, -5,
						   -3, -1, 1});
		}

	SECTION("Scalar multiplication")
	{
		auto c = a * 2.0f;
		REQUIRE(c == Mat3f{2, 4, 6,
						   8, 10, 12,
						   14, 16, 18});
	}

	SECTION("Scalar division")
	{
		auto c = b / 2.0f;
		REQUIRE(c == Mat3f{8.0f, 7.5f, 7.0f,
						   6.5f, 6.0f, 5.5f,
						   5.0f, 4.5f, 4.0f});
	}

	SECTION("Matrix multiplication")
	{
		Mat3f identity = Mat3f::identity();
		auto c = a * identity;
		REQUIRE(c == a);
	}

	SECTION("Vector multiplication")
	{
		Vec<float, 3> v(1, 2, 3);
		auto r = a * v;
		REQUIRE(r.x == Catch::Approx(14.0f));
		REQUIRE(r.y == Catch::Approx(32.0f));
		REQUIRE(r.z == Catch::Approx(50.0f));
	}

	SECTION("Compound assignment")
	{
		Mat3f c = a;
		c += b;
		REQUIRE(c[0][0] == Catch::Approx(17.0f));
		REQUIRE(c[2][2] == Catch::Approx(17.0f));

		c -= a;
		REQUIRE(c == b);

		c *= 2.0f;
		REQUIRE(c[0][0] == Catch::Approx(32.0f));
		REQUIRE(c[2][2] == Catch::Approx(16.0f));

		c /= 2.0f;
		REQUIRE(c == b);
	}
}

TEST_CASE("Mat3x3 transformations", "[Mat3x3]")
{
	SECTION("Identity")
	{
		REQUIRE(Mat3f::identity() == Mat3f{1, 0, 0,
										   0, 1, 0,
										   0, 0, 1});
	}

	SECTION("Transpose")
	{
		 constexpr Mat3f m(1, 2, 3,
						   4, 5, 6,
						   7, 8, 9);
		REQUIRE(m.transpose() == Mat3f{1, 4, 7,
									   2, 5, 8,
									   3, 6, 9});
	}
}

TEST_CASE("Mat3x3 comparison", "[Mat3x3]")
{
	Mat3f a(1, 2, 3,
			4, 5, 6,
			7, 8, 9);
	Mat3f b(1, 2, 3,
			4, 5, 6,
			7, 8, 9);
	Mat3f c(16, 15, 14,
			13, 12, 11,
			10, 9, 8);

	REQUIRE(a == b);
	REQUIRE(a != c);
}

TEST_CASE("Mat3x3 inverse", "[Mat3x3]")
{
    SECTION("Inverse of identity matrix")
    {
        Mat3f identity = Mat3f::identity();
        REQUIRE(Mat3f::inverse(identity) == identity);
    }

    SECTION("Inverse of scale matrix (Mat input)")
    {
        Mat3f scale(2.0f, 0, 0,
                    0, 3.0f, 0,
                    0, 0, 4.0f);
        Mat3f inv_scale = Mat3f::inverse(scale);
        Mat3f expected_inv_scale(0.5f, 0, 0,
                                 0, 1.0f/3.0f, 0,
                                 0, 0, 0.25f);
        REQUIRE(inv_scale[0][0] == Catch::Approx(expected_inv_scale[0][0]));
        REQUIRE(inv_scale[0][1] == Catch::Approx(expected_inv_scale[0][1]));
        REQUIRE(inv_scale[0][2] == Catch::Approx(expected_inv_scale[0][2]));
        REQUIRE(inv_scale[1][0] == Catch::Approx(expected_inv_scale[1][0]));
        REQUIRE(inv_scale[1][1] == Catch::Approx(expected_inv_scale[1][1]));
        REQUIRE(inv_scale[1][2] == Catch::Approx(expected_inv_scale[1][2]));
        REQUIRE(inv_scale[2][0] == Catch::Approx(expected_inv_scale[2][0]));
        REQUIRE(inv_scale[2][1] == Catch::Approx(expected_inv_scale[2][1]));
        REQUIRE(inv_scale[2][2] == Catch::Approx(expected_inv_scale[2][2]));
    }

    SECTION("Inverse of scale matrix (x, y, z input)")
    {
        Mat3f inv_scale = Mat3f::inverse(2.0f, 3.0f, 4.0f);
        Mat3f expected_inv_scale{0.5, 0, 0,
                                 0, 1.0/3.0, 0,
                                 0, 0, 0.25};
        REQUIRE(inv_scale[0][0] == Catch::Approx(expected_inv_scale[0][0]));
        REQUIRE(inv_scale[0][1] == Catch::Approx(expected_inv_scale[0][1]));
        REQUIRE(inv_scale[0][2] == Catch::Approx(expected_inv_scale[0][2]));
        REQUIRE(inv_scale[1][0] == Catch::Approx(expected_inv_scale[1][0]));
        REQUIRE(inv_scale[1][1] == Catch::Approx(expected_inv_scale[1][1]));
        REQUIRE(inv_scale[1][2] == Catch::Approx(expected_inv_scale[1][2]));
        REQUIRE(inv_scale[2][0] == Catch::Approx(expected_inv_scale[2][0]));
        REQUIRE(inv_scale[2][1] == Catch::Approx(expected_inv_scale[2][1]));
        REQUIRE(inv_scale[2][2] == Catch::Approx(expected_inv_scale[2][2]));
    }

    SECTION("Inverse of rotation matrix")
    {
        Mat3f rot = Mat3f::rotateZ(PI_F_2);
        Mat3f inv_rot = Mat3f::inverse(rot);
        Mat3f product = rot * inv_rot;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                REQUIRE(product[i][j] == Catch::Approx(i == j ? 1.0f : 0.0f));
    }

    SECTION("Inverse of general matrix")
    {
	    Mat3f m{1, 2, 3,
				0, 1, 4,
				5, 6, 0};
        Mat3f inv_m = Mat3f::inverse(m);
        Mat3f product = m * inv_m;
    	Mat3f expected{1, 0, 0,
    				   0, 1, 0,
    				   0, 0, 1};
    	REQUIRE(product == expected);
    }
}
