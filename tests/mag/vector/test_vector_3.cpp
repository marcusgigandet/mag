#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>

import mag.vector;
using namespace mag;

TEST_CASE("Vec3 basic arithmetic", "[Vec3]")
{
	Vec<float, 3> a{1.0f, 2.0f, 3.0f};
	Vec<int, 3> b{3, 4, 5};

	SECTION("Addition")
	{
		auto c = a + b;
		REQUIRE(c[0] == Catch::Approx(4.0f));
		REQUIRE(c[1] == Catch::Approx(6.0f));
		REQUIRE(c[2] == Catch::Approx(8.0f));
	}

	SECTION("Subtraction")
	{
		auto c = b - a;
		REQUIRE(c[0] == Catch::Approx(2.0f));
		REQUIRE(c[1] == Catch::Approx(2.0f));
		REQUIRE(c[2] == Catch::Approx(2.0f));

		c = a - b;
		REQUIRE(c[0] == Catch::Approx(-2.0f));
		REQUIRE(c[1] == Catch::Approx(-2.0f));
		REQUIRE(c[2] == Catch::Approx(-2.0f));
	}

	SECTION("Scalar multiplication")
	{
		auto c = a * 2.0f;
		REQUIRE(c[0] == Catch::Approx(2.0f));
		REQUIRE(c[1] == Catch::Approx(4.0f));
		REQUIRE(c[2] == Catch::Approx(6.0f));
	}

	SECTION("Scalar division")
	{
		auto c = b / 2.0f;
		REQUIRE(c[0] == Catch::Approx(1.5f));
		REQUIRE(c[1] == Catch::Approx(2.0f));
		REQUIRE(c[2] == Catch::Approx(2.5f));
	}

	SECTION("Element-wise multiplication")
	{
		auto c = a * b;
		REQUIRE(c[0] == Catch::Approx(3.0f));
		REQUIRE(c[1] == Catch::Approx(8.0f));
		REQUIRE(c[2] == Catch::Approx(15.0f));
	}

	SECTION("Element-wise division")
	{
		Vec<float, 3> d{4.0f, 8.0f, 12.0f};
		auto c = d / a;
		REQUIRE(c[0] == Catch::Approx(4.0f));
		REQUIRE(c[1] == Catch::Approx(4.0f));
		REQUIRE(c[2] == Catch::Approx(4.0f));
	}
}

TEST_CASE("Vec3 length, dot, normalize", "[Vec3]")
{
	Vec<float, 3> a{1.0f, 2.0f, 3.0f};

	SECTION("Length") { REQUIRE(a.length() == Catch::Approx(std::sqrt(14.0f))); }

	SECTION("Dot product")
	{
		Vec<float, 3> b{2.0f, 3.0f, 4.0f};
		REQUIRE(a.dot(b) == Catch::Approx(20.0f));
	}

	SECTION("Normalization")
	{
		auto normalized = a.normalized();
		REQUIRE(normalized.length() == Catch::Approx(1.0f));
	}
}

TEST_CASE("Vec3 clamp and lerp", "[Vec3]")
{
	Vec<float, 3> a{1.0f, 2.0f, 3.0f};
	Vec<float, 3> b{3.0f, 4.0f, 5.0f};

	SECTION("Clamp")
	{
		auto clamped = a.clamped(1.5f, 2.5f);
		REQUIRE(clamped[0] == Catch::Approx(1.5f));
		REQUIRE(clamped[1] == Catch::Approx(2.0f));
		REQUIRE(clamped[2] == Catch::Approx(2.5f));
	}

	SECTION("Lerp")
	{
		auto lerped = lerp(a, b, 0.5f);
		REQUIRE(lerped[0] == Catch::Approx(2.0f));
		REQUIRE(lerped[1] == Catch::Approx(3.0f));
		REQUIRE(lerped[2] == Catch::Approx(4.0f));
	}
}

TEST_CASE("Vec3 comparison", "[Vec3]")
{
	Vec<float, 3> a{1.0f, 2.0f, 3.0f};
	Vec<float, 3> b{1.0f, 2.0f, 3.0f};
	Vec<float, 3> c{3.0f, 4.0f, 5.0f};

	REQUIRE(a == b);
	REQUIRE(a != c);
	REQUIRE((a <=> b) == std::partial_ordering::equivalent);
	REQUIRE((a <=> c) != std::partial_ordering::equivalent);
}

TEST_CASE("Vec3 compound arithmetic and scalar-first operations", "[Vec3]")
{
	SECTION("Compound vector operations")
	{
		Vec<float, 3> value{6.0f, 9.0f, 12.0f};
		Vec<float, 3> delta{1.0f, 3.0f, 4.0f};

		value += delta;
		REQUIRE(value[0] == Catch::Approx(7.0f));
		REQUIRE(value[1] == Catch::Approx(12.0f));
		REQUIRE(value[2] == Catch::Approx(16.0f));

		value -= delta;
		REQUIRE(value[0] == Catch::Approx(6.0f));
		REQUIRE(value[1] == Catch::Approx(9.0f));
		REQUIRE(value[2] == Catch::Approx(12.0f));

		value *= delta;
		REQUIRE(value[0] == Catch::Approx(6.0f));
		REQUIRE(value[1] == Catch::Approx(27.0f));
		REQUIRE(value[2] == Catch::Approx(48.0f));

		value /= delta;
		REQUIRE(value[0] == Catch::Approx(6.0f));
		REQUIRE(value[1] == Catch::Approx(9.0f));
		REQUIRE(value[2] == Catch::Approx(12.0f));
	}

	SECTION("Compound scalar operations")
	{
		Vec<float, 3> value{2.0f, 3.0f, 4.0f};

		value += 2.0f;
		REQUIRE(value[0] == Catch::Approx(4.0f));
		REQUIRE(value[1] == Catch::Approx(5.0f));
		REQUIRE(value[2] == Catch::Approx(6.0f));

		value -= 1.0f;
		REQUIRE(value[0] == Catch::Approx(3.0f));
		REQUIRE(value[1] == Catch::Approx(4.0f));
		REQUIRE(value[2] == Catch::Approx(5.0f));

		value *= 2.0f;
		REQUIRE(value[0] == Catch::Approx(6.0f));
		REQUIRE(value[1] == Catch::Approx(8.0f));
		REQUIRE(value[2] == Catch::Approx(10.0f));

		value /= 2.0f;
		REQUIRE(value[0] == Catch::Approx(3.0f));
		REQUIRE(value[1] == Catch::Approx(4.0f));
		REQUIRE(value[2] == Catch::Approx(5.0f));
	}

	SECTION("Scalar-first operators")
	{
		Vec<float, 3> value{1.0f, 2.0f, 3.0f};

		auto sum = 2.0f + value;
		REQUIRE(sum[0] == Catch::Approx(3.0f));
		REQUIRE(sum[1] == Catch::Approx(4.0f));
		REQUIRE(sum[2] == Catch::Approx(5.0f));

		auto diff = 10.0f - value;
		REQUIRE(diff[0] == Catch::Approx(9.0f));
		REQUIRE(diff[1] == Catch::Approx(8.0f));
		REQUIRE(diff[2] == Catch::Approx(7.0f));

		auto prod = 3.0f * value;
		REQUIRE(prod[0] == Catch::Approx(3.0f));
		REQUIRE(prod[1] == Catch::Approx(6.0f));
		REQUIRE(prod[2] == Catch::Approx(9.0f));
	}
}

TEST_CASE("Vec3 cross product, aliases, and iterators", "[Vec3]")
{
	SECTION("Free and member cross product")
	{
		Vec<float, 3> a{1.0f, 0.0f, 0.0f};
		Vec<float, 3> b{0.0f, 1.0f, 0.0f};

		auto freeCross = cross(a, b);
		REQUIRE(freeCross[0] == Catch::Approx(0.0f));
		REQUIRE(freeCross[1] == Catch::Approx(0.0f));
		REQUIRE(freeCross[2] == Catch::Approx(1.0f));

		auto memberCross = a.cross(b);
		REQUIRE(memberCross[0] == Catch::Approx(0.0f));
		REQUIRE(memberCross[1] == Catch::Approx(0.0f));
		REQUIRE(memberCross[2] == Catch::Approx(1.0f));
	}

	SECTION("Aliases and iterators")
	{
		Vec<float, 3> color{1.0f, 2.0f, 3.0f};
		color.r = 4.0f;
		color.g = 5.0f;
		color.b = 6.0f;
		REQUIRE(color.x == Catch::Approx(4.0f));
		REQUIRE(color.y == Catch::Approx(5.0f));
		REQUIRE(color.z == Catch::Approx(6.0f));

		float sum = 0.0f;
		for (float c : color)
			sum += c;
		REQUIRE(sum == Catch::Approx(15.0f));

		auto rev = color.rbegin();
		REQUIRE(*rev == Catch::Approx(6.0f));
		++rev;
		REQUIRE(*rev == Catch::Approx(5.0f));
	}
}
