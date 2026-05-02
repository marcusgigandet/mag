#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>

import mag;
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
	// REQUIRE((a <=> b) == std::partial_ordering::equivalent);
	// REQUIRE((a <=> c) != std::partial_ordering::equivalent);
}
