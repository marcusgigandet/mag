#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>

import mag;
using namespace mag;

TEST_CASE("Vec4 basic arithmetic", "[Vec4]")
{
	Vec<float, 4> a{1.0f, 2.0f, 3.0f, 4.0f};
	Vec<int, 4> b{3, 4, 5, 6};

	SECTION("Addition")
	{
		auto c = a + b;
		REQUIRE(c[0] == Catch::Approx(4.0f));
		REQUIRE(c[1] == Catch::Approx(6.0f));
		REQUIRE(c[2] == Catch::Approx(8.0f));
		REQUIRE(c[3] == Catch::Approx(10.0f));
	}

	SECTION("Subtraction")
	{
		auto c = b - a;
		REQUIRE(c[0] == Catch::Approx(2.0f));
		REQUIRE(c[1] == Catch::Approx(2.0f));
		REQUIRE(c[2] == Catch::Approx(2.0f));
		REQUIRE(c[3] == Catch::Approx(2.0f));

		c = a - b;
		REQUIRE(c[0] == Catch::Approx(-2.0f));
		REQUIRE(c[1] == Catch::Approx(-2.0f));
		REQUIRE(c[2] == Catch::Approx(-2.0f));
		REQUIRE(c[3] == Catch::Approx(-2.0f));
	}

	SECTION("Scalar multiplication")
	{
		auto c = a * 2.0f;
		REQUIRE(c[0] == Catch::Approx(2.0f));
		REQUIRE(c[1] == Catch::Approx(4.0f));
		REQUIRE(c[2] == Catch::Approx(6.0f));
		REQUIRE(c[3] == Catch::Approx(8.0f));
	}

	SECTION("Scalar division")
	{
		auto c = b / 2.0f;
		REQUIRE(c[0] == Catch::Approx(1.5f));
		REQUIRE(c[1] == Catch::Approx(2.0f));
		REQUIRE(c[2] == Catch::Approx(2.5f));
		REQUIRE(c[3] == Catch::Approx(3.0f));
	}

	SECTION("Element-wise multiplication")
	{
		auto c = a * b;
		REQUIRE(c[0] == Catch::Approx(3.0f));
		REQUIRE(c[1] == Catch::Approx(8.0f));
		REQUIRE(c[2] == Catch::Approx(15.0f));
		REQUIRE(c[3] == Catch::Approx(24.0f));
	}

	SECTION("Element-wise division")
	{
		Vec<float, 4> d{4.0f, 8.0f, 12.0f, 16.0f};
		auto c = d / a;
		REQUIRE(c[0] == Catch::Approx(4.0f));
		REQUIRE(c[1] == Catch::Approx(4.0f));
		REQUIRE(c[2] == Catch::Approx(4.0f));
		REQUIRE(c[3] == Catch::Approx(4.0f));
	}
}

TEST_CASE("Vec4 length, dot, normalize", "[Vec4]")
{
	Vec<float, 4> a{1.0f, 2.0f, 3.0f, 4.0f};

	SECTION("Length") { REQUIRE(a.length() == Catch::Approx(std::sqrt(30.0f))); }

	SECTION("Dot product")
	{
		Vec<float, 4> b{2.0f, 3.0f, 4.0f, 5.0f};
		REQUIRE(a.dot(b) == Catch::Approx(40.0f));
	}

	SECTION("Normalization")
	{
		auto normalized = a.normalized();
		REQUIRE(normalized.length() == Catch::Approx(1.0f));
	}
}

TEST_CASE("Vec4 clamp and lerp", "[Vec4]")
{
	Vec<float, 4> a{1.0f, 2.0f, 3.0f, 4.0f};
	Vec<float, 4> b{3.0f, 4.0f, 5.0f, 6.0f};

	SECTION("Clamp")
	{
		auto clamped = a.clamped(1.5f, 3.5f);
		REQUIRE(clamped[0] == Catch::Approx(1.5f));
		REQUIRE(clamped[1] == Catch::Approx(2.0f));
		REQUIRE(clamped[2] == Catch::Approx(3.0f));
		REQUIRE(clamped[3] == Catch::Approx(3.5f));
	}

	SECTION("Lerp")
	{
		auto lerped = lerp(a, b, 0.5f);
		REQUIRE(lerped[0] == Catch::Approx(2.0f));
		REQUIRE(lerped[1] == Catch::Approx(3.0f));
		REQUIRE(lerped[2] == Catch::Approx(4.0f));
		REQUIRE(lerped[3] == Catch::Approx(5.0f));
	}
}

TEST_CASE("Vec4 comparison", "[Vec4]")
{
	Vec<float, 4> a{1.0f, 2.0f, 3.0f, 4.0f};
	Vec<float, 4> b{1.0f, 2.0f, 3.0f, 4.0f};
	Vec<float, 4> c{3.0f, 4.0f, 5.0f, 6.0f};

	REQUIRE(a == b);
	REQUIRE(a != c);
	REQUIRE((a <=> b) == std::partial_ordering::equivalent);
	REQUIRE((a <=> c) != std::partial_ordering::equivalent);
}

TEST_CASE("Vec4 constructors and aliases", "[Vec4]")
{
	SECTION("Scalar constructor splats all components")
	{
		Vec<float, 4> v{2.5f};
		REQUIRE(v.x == Catch::Approx(2.5f));
		REQUIRE(v.y == Catch::Approx(2.5f));
		REQUIRE(v.z == Catch::Approx(2.5f));
		REQUIRE(v.w == Catch::Approx(2.5f));
	}

	SECTION("Mixed numeric constructor converts values")
	{
		Vec<float, 4> v{1, 2.5, 3U, -4};
		REQUIRE(v[0] == Catch::Approx(1.0f));
		REQUIRE(v[1] == Catch::Approx(2.5f));
		REQUIRE(v[2] == Catch::Approx(3.0f));
		REQUIRE(v[3] == Catch::Approx(-4.0f));
	}

	SECTION("RGBA aliases map to XYZW storage")
	{
		Vec<float, 4> color{1.0f, 2.0f, 3.0f, 4.0f};
		color.r = 5.0f;
		color.g = 6.0f;
		color.b = 7.0f;
		color.a = 8.0f;

		REQUIRE(color.x == Catch::Approx(5.0f));
		REQUIRE(color.y == Catch::Approx(6.0f));
		REQUIRE(color.z == Catch::Approx(7.0f));
		REQUIRE(color.w == Catch::Approx(8.0f));
	}
}

TEST_CASE("Vec4 compound operators and scalar-left overloads", "[Vec4]")
{
	SECTION("Vector compound operators")
	{
		Vec<float, 4> value{8.0f, 12.0f, 16.0f, 20.0f};
		Vec<float, 4> delta{2.0f, 3.0f, 4.0f, 5.0f};

		value += delta;
		REQUIRE(value[0] == Catch::Approx(10.0f));
		REQUIRE(value[1] == Catch::Approx(15.0f));
		REQUIRE(value[2] == Catch::Approx(20.0f));
		REQUIRE(value[3] == Catch::Approx(25.0f));

		value -= delta;
		REQUIRE(value[0] == Catch::Approx(8.0f));
		REQUIRE(value[1] == Catch::Approx(12.0f));
		REQUIRE(value[2] == Catch::Approx(16.0f));
		REQUIRE(value[3] == Catch::Approx(20.0f));

		value *= delta;
		REQUIRE(value[0] == Catch::Approx(16.0f));
		REQUIRE(value[1] == Catch::Approx(36.0f));
		REQUIRE(value[2] == Catch::Approx(64.0f));
		REQUIRE(value[3] == Catch::Approx(100.0f));

		value /= delta;
		REQUIRE(value[0] == Catch::Approx(8.0f));
		REQUIRE(value[1] == Catch::Approx(12.0f));
		REQUIRE(value[2] == Catch::Approx(16.0f));
		REQUIRE(value[3] == Catch::Approx(20.0f));
	}

	SECTION("Scalar compound operators")
	{
		Vec<float, 4> value{2.0f, 3.0f, 4.0f, 5.0f};

		value += 1.0f;
		value -= 2.0f;
		value *= 2.0f;
		value /= 2.0f;

		REQUIRE(value[0] == Catch::Approx(1.0f));
		REQUIRE(value[1] == Catch::Approx(2.0f));
		REQUIRE(value[2] == Catch::Approx(3.0f));
		REQUIRE(value[3] == Catch::Approx(4.0f));
	}

	SECTION("Scalar-left operators")
	{
		Vec<float, 4> v{1.0f, 2.0f, 3.0f, 4.0f};
		auto sum = 10.0f + v;
		auto diff = 10.0f - v;
		auto prod = 0.5f * v;

		REQUIRE(sum[0] == Catch::Approx(11.0f));
		REQUIRE(sum[1] == Catch::Approx(12.0f));
		REQUIRE(sum[2] == Catch::Approx(13.0f));
		REQUIRE(sum[3] == Catch::Approx(14.0f));

		REQUIRE(diff[0] == Catch::Approx(9.0f));
		REQUIRE(diff[1] == Catch::Approx(8.0f));
		REQUIRE(diff[2] == Catch::Approx(7.0f));
		REQUIRE(diff[3] == Catch::Approx(6.0f));

		REQUIRE(prod[0] == Catch::Approx(0.5f));
		REQUIRE(prod[1] == Catch::Approx(1.0f));
		REQUIRE(prod[2] == Catch::Approx(1.5f));
		REQUIRE(prod[3] == Catch::Approx(2.0f));
	}
}

TEST_CASE("Vec4 utility helpers and iterators", "[Vec4]")
{
	SECTION("Distance and normalized zero vector")
	{
		Vec<float, 4> a{1.0f, 2.0f, 3.0f, 4.0f};
		Vec<float, 4> b{2.0f, 4.0f, 6.0f, 8.0f};
		REQUIRE(distance(a, b) == Catch::Approx(std::sqrt(30.0f)));

		Vec<float, 4> zero{0.0f, 0.0f, 0.0f, 0.0f};
		auto normalized = zero.normalized();
		REQUIRE(normalized[0] == Catch::Approx(0.0f));
		REQUIRE(normalized[1] == Catch::Approx(0.0f));
		REQUIRE(normalized[2] == Catch::Approx(0.0f));
		REQUIRE(normalized[3] == Catch::Approx(0.0f));
	}

	SECTION("toString formatting")
	{
		Vec<float, 4> v{1.0f, 2.0f, 3.0f, 4.0f};
		REQUIRE(v.toString() == "Vec4(1, 2, 3, 4)");
	}

	SECTION("Iterators and reverse iterators")
	{
		Vec<float, 4> v{1.0f, 2.0f, 3.0f, 4.0f};
		float forwardSum = 0.0f;
		for (float c : v)
			forwardSum += c;
		REQUIRE(forwardSum == Catch::Approx(10.0f));

		auto rit = v.rbegin();
		REQUIRE(*rit == Catch::Approx(4.0f));
		++rit;
		REQUIRE(*rit == Catch::Approx(3.0f));
	}
}
