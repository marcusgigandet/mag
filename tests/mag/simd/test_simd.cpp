#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <array>
#include <iostream>
#include <span>

import mag;
using namespace mag;
using namespace mag::simd;

namespace
{
	constexpr Vec4f toVec(const f32x4& v)
	{
		Vec4f out{};
		v.store(out.v);
		return out;
	}
} // namespace

TEST_CASE("f32x basic arithmetic", "[simd]")
{
	f32x a{1.0f};
	f32x b{1.0f, 2.0f, 3.0f, 4.0f};

	SECTION("addition")
	{
		auto c = a + b;

		REQUIRE(toVec(c) == Vec4f{2.0f, 3.0f, 4.0f, 5.0f});
	}

	SECTION("subtraction")
	{
		auto c = a - b;

		REQUIRE(toVec(c) == Vec4f{0.0f, -1.0f, -2.0f, -3.0f});
	}

	SECTION("multiplication")
	{
		auto c = a * b;

		REQUIRE(toVec(c) == Vec4f{1.0f, 2.0f, 3.0f, 4.0f});
	}

	SECTION("division")
	{
		auto c = a / b;
		Vec4f r = toVec(c);

		REQUIRE(r.x == Catch::Approx(1.0f));
		REQUIRE(r.y == Catch::Approx(0.5f));
		REQUIRE(r.z == Catch::Approx(1.0f / 3.0f));
		REQUIRE(r.w == Catch::Approx(0.25f));
	}
}

TEST_CASE("f32x compound arithmetic", "[simd]")
{
	f32x a{1.0f};
	f32x b{1.0f, 2.0f, 3.0f, 4.0f};

	SECTION("addition assignment")
	{
		f32x c = b;
		c += a;

		REQUIRE(toVec(c) == Vec4f{2.0f, 3.0f, 4.0f, 5.0f});
	}

	SECTION("subtraction assignment")
	{
		f32x c = b;
		c -= a;

		REQUIRE(toVec(c) == Vec4f{0.0f, 1.0f, 2.0f, 3.0f});
	}

	SECTION("multiplication assignment")
	{
		f32x c = b;
		c *= a;

		REQUIRE(toVec(c) == Vec4f{1.0f, 2.0f, 3.0f, 4.0f});
	}

	SECTION("division assignment")
	{
		f32x c = b;
		c /= a;

		Vec4f r = toVec(c);

		REQUIRE(r.x == Catch::Approx(1.0f));
		REQUIRE(r.y == Catch::Approx(2.0f));
		REQUIRE(r.z == Catch::Approx(3.0f));
		REQUIRE(r.w == Catch::Approx(4.0f));
	}
}

TEST_CASE("f32x4 load/store/splat/hsum", "[simd]")
{
	f32x a{1.0f};
	f32x b{1.0f, 2.0f, 3.0f, 4.0f};

	SECTION("load from args")
	{
		f32x c{5.0f, 6.0f, 7.0f, 8.0f};

		REQUIRE(toVec(c) == Vec4f{5.0f, 6.0f, 7.0f, 8.0f});
	}

	SECTION("load from pointer")
	{
		constexpr float data[4]{5.0f, 6.0f, 7.0f, 8.0f};

		f32x c{data};

		REQUIRE(toVec(c) == Vec4f{5.0f, 6.0f, 7.0f, 8.0f});
	}

	SECTION("load from span")
	{
		float data[4]{5.0f, 6.0f, 7.0f, 8.0f};
		std::span s{data};

		f32x c{s};

		REQUIRE(toVec(c) == Vec4f{5.0f, 6.0f, 7.0f, 8.0f});
	}

	SECTION("store to pointer")
	{
		// Check that convertable types are allowed
		f32x c{5.0f, 6.0f, 7.0, 8.0};

		std::array<float, 4> out{};
		c.store(out.data());

		REQUIRE(out == std::array<float, 4>{5.0f, 6.0f, 7.0f, 8.0f});
	}

	SECTION("horizontal sum") { REQUIRE(hsum(a + b) == Catch::Approx(2.0f + 3.0f + 4.0f + 5.0f)); }

	SECTION("horizontal min")
	{
		f32x b{1.0f, 2.0f, 3.0f, 4.0f};

		REQUIRE(hmin(b) == Catch::Approx(1.0f));

		f32x c{4.0f, -2.0f, 7.0f, 3.0f};
		REQUIRE(hmin(c) == Catch::Approx(-2.0f));

		f32x d{-1.0f, -5.0f, -3.0f, -4.0f};
		REQUIRE(hmin(d) == Catch::Approx(-5.0f));

		f32x e{0.0f, 0.0f, 0.0f, 0.0f};
		REQUIRE(hmin(e) == Catch::Approx(0.0f));
	}

	SECTION("horizontal max")
	{
		f32x b{1.0f, 2.0f, 3.0f, 4.0f};
		REQUIRE(hmax(b) == Catch::Approx(4.0f));

		f32x c{4.0f, -2.0f, 7.0f, 3.0f};
		REQUIRE(hmax(c) == Catch::Approx(7.0f));

		f32x d{-1.0f, -5.0f, -3.0f, -4.0f};
		REQUIRE(hmax(d) == Catch::Approx(-1.0f));

		f32x e{0.0f, 0.0f, 0.0f, 0.0f};
		REQUIRE(hmax(e) == Catch::Approx(0.0f));
	}

	f32x c{5, 6, 8, 12};
	f32x d{3, 6, 3, 9};

	// auto f = sqrt(max(c, d));
	// float array[4];

	// f.store(array);

	// for (size_t i = 0; i < 4; i++)
	// 	std::cout << array[i] << std::endl;
	// std::cout << std::endl;
}
