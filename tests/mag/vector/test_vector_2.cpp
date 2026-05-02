#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

import mag;
using namespace mag;

TEST_CASE("Vec2 basic arithmetic", "[Vec2]")
{
	Vec<float, 2> a{1.0f, 2.0f};
	Vec<int, 2> b{3, 4};

	SECTION("Addition")
	{
		auto c = a + b;
		REQUIRE(c[0] == Catch::Approx(4.0f));
		REQUIRE(c[1] == Catch::Approx(6.0f));
	}

	SECTION("Subtraction")
	{
		auto c = b - a;
		REQUIRE(c[0] == Catch::Approx(2.0f));
		REQUIRE(c[1] == Catch::Approx(2.0f));

		c = a - b;
		REQUIRE(c[0] == Catch::Approx(-2.0f));
		REQUIRE(c[1] == Catch::Approx(-2.0f));
	}

	SECTION("Scalar multiplication")
	{
		auto c = a * 2.0f;
		REQUIRE(c[0] == Catch::Approx(2.0f));
		REQUIRE(c[1] == Catch::Approx(4.0f));
	}

	SECTION("Scalar division")
	{
		auto c = b / 2.0f;
		REQUIRE(c[0] == Catch::Approx(1.5f));
		REQUIRE(c[1] == Catch::Approx(2.0f));
	}

	SECTION("Element-wise multiplication")
	{
		auto c = a * b;
		REQUIRE(c[0] == Catch::Approx(3.0f));
		REQUIRE(c[1] == Catch::Approx(8.0f));
	}

	SECTION("Element-wise division")
	{
		Vec<float, 2> d{4.0f, 8.0f};
		auto c = d / a;
		REQUIRE(c[0] == Catch::Approx(4.0f));
		REQUIRE(c[1] == Catch::Approx(4.0f));
	}
}

TEST_CASE("Vec2 length, dot, normalize", "[Vec2]")
{
	Vec<float, 2> a{3.0f, 4.0f};

	SECTION("Length") { REQUIRE(a.length() == Catch::Approx(5.0f)); }

	SECTION("Dot product")
	{
		Vec<float, 2> b{2.0f, 3.0f};
		REQUIRE(a.dot(b) == Catch::Approx(18.0f));
	}

	SECTION("Normalization")
	{
		auto normalized = a.normalized();
		REQUIRE(normalized.length() == Catch::Approx(1.0f));
		REQUIRE(normalized[0] == Catch::Approx(0.6f));
		REQUIRE(normalized[1] == Catch::Approx(0.8f));
	}
}

TEST_CASE("Vec2 clamp and lerp", "[Vec2]")
{
	Vec<float, 2> a{1.0f, 2.0f};
	Vec<float, 2> b{3.0f, 4.0f};

	SECTION("Clamp")
	{
		auto clamped = a.clamped(1.5f, 2.5f);
		REQUIRE(clamped[0] == Catch::Approx(1.5f));
		REQUIRE(clamped[1] == Catch::Approx(2.0f));
	}

	SECTION("Lerp")
	{
		auto lerped = lerp(a, b, 0.5f);
		REQUIRE(lerped[0] == Catch::Approx(2.0f));
		REQUIRE(lerped[1] == Catch::Approx(3.0f));
	}
}

TEST_CASE("Vec2 comparison", "[Vec2]")
{
	Vec<float, 2> a{1.0f, 2.0f};
	Vec<float, 2> b{1.0f, 2.0f};
	Vec<float, 2> c{3.0f, 4.0f};

	REQUIRE(a == b);
	REQUIRE(a != c);
	// REQUIRE((a <=> b) == std::partial_ordering::equivalent);
	// REQUIRE((a <=> c) != std::partial_ordering::equivalent);
}
