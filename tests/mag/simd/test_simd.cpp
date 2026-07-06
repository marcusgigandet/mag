#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <array>
#include <cmath>
#include <concepts>
#include <span>
#include <utility>

import mag.simd;
using namespace mag::simd;

namespace
{
	template <typename T, size_t N>
	std::array<T, N> toArray(const Simd<T, N>& v)
	{
		std::array<T, N> out{};
		v.store(out.data());
		return out;
	}

	template <typename T, size_t N>
	void requireArrayEquals(const std::array<T, N>& actual, const std::array<T, N>& expected)
	{
		for (size_t i = 0; i < N; ++i)
		{
			if constexpr (std::floating_point<T>)
			{
				REQUIRE(actual[i] == Catch::Approx(expected[i]));
			}
			else
			{
				REQUIRE(actual[i] == expected[i]);
			}
		}
	}

	template <typename T, size_t N>
	void requireSimdEquals(const Simd<T, N>& actual, const std::array<T, N>& expected)
	{
		requireArrayEquals(toArray(actual), expected);
	}
} // namespace

TEST_CASE("simd compile-time contracts", "[simd]")
{
	STATIC_REQUIRE(f32x4::size() == 4);
	STATIC_REQUIRE(f64x2::size() == 2);
	STATIC_REQUIRE(f32x::size() == abi_lanes_v<float, native_abi>);
	STATIC_REQUIRE(f64x::size() == abi_lanes_v<double, native_abi>);

	STATIC_REQUIRE(abi_lanes_v<float, scalar_abi> == 1);
	STATIC_REQUIRE(abi_lanes_v<float, fixed_abi<4>> == 4);
	STATIC_REQUIRE(abi_lanes_v<double, fixed_abi<2>> == 2);

	STATIC_REQUIRE(std::same_as<basic_simd<float, native_abi>, native_simd<float>>);
	STATIC_REQUIRE(std::same_as<fixed_simd<float, 4>, Simd<float, 4>>);
	STATIC_REQUIRE(std::same_as<decltype(load<float, fixed_abi<4>>(std::declval<const float*>())),
								fixed_simd<float, 4>>);
	STATIC_REQUIRE(std::same_as<decltype(splat<float, fixed_abi<4>>(1.0f)), fixed_simd<float, 4>>);

	STATIC_REQUIRE(supports_add<float, 4>);
	STATIC_REQUIRE(supports_sub<float, 4>);
	STATIC_REQUIRE(supports_mul<float, 4>);
	STATIC_REQUIRE(supports_div<float, 4>);
	STATIC_REQUIRE(supports_reduction<float, 4>);
	STATIC_REQUIRE(supports_add<int32_t, 4>);
	STATIC_REQUIRE(supports_sub<uint32_t, 4>);
	STATIC_REQUIRE(!supports_div<int32_t, 4>);
}

TEST_CASE("f32x arithmetic supports simd and scalar operands", "[simd]")
{
	const f32x4 a{2.0f, -4.0f, 8.0f, -16.0f};
	const f32x4 b{1.0f, 2.0f, -4.0f, -8.0f};

	SECTION("simd operands")
	{
		requireSimdEquals(a + b, {3.0f, -2.0f, 4.0f, -24.0f});
		requireSimdEquals(a - b, {1.0f, -6.0f, 12.0f, -8.0f});
		requireSimdEquals(a * b, {2.0f, -8.0f, -32.0f, 128.0f});
		requireSimdEquals(a / b, {2.0f, -2.0f, -2.0f, 2.0f});
	}

	SECTION("simd left scalar right")
	{
		requireSimdEquals(a + 2.0f, {4.0f, -2.0f, 10.0f, -14.0f});
		requireSimdEquals(a - 2.0f, {0.0f, -6.0f, 6.0f, -18.0f});
		requireSimdEquals(a * -0.5f, {-1.0f, 2.0f, -4.0f, 8.0f});
		requireSimdEquals(a / -2.0f, {-1.0f, 2.0f, -4.0f, 8.0f});
	}

	SECTION("scalar left simd right")
	{
		requireSimdEquals(2.0f + b, {3.0f, 4.0f, -2.0f, -6.0f});
		requireSimdEquals(2.0f - b, {1.0f, 0.0f, 6.0f, 10.0f});
		requireSimdEquals(-0.5f * b, {-0.5f, -1.0f, 2.0f, 4.0f});
		requireSimdEquals(-16.0f / b, {-16.0f, -8.0f, 4.0f, 2.0f});
	}
}

TEST_CASE("f32x compound arithmetic and helpers", "[simd]")
{
	SECTION("compound simd operands")
	{
		f32x4 value{2.0f, -4.0f, 8.0f, -16.0f};
		const f32x4 delta{1.0f, 2.0f, -4.0f, -8.0f};

		value += delta;
		requireSimdEquals(value, {3.0f, -2.0f, 4.0f, -24.0f});

		value -= delta;
		requireSimdEquals(value, {2.0f, -4.0f, 8.0f, -16.0f});

		value *= delta;
		requireSimdEquals(value, {2.0f, -8.0f, -32.0f, 128.0f});

		value /= delta;
		requireSimdEquals(value, {2.0f, -4.0f, 8.0f, -16.0f});
	}

	SECTION("compound scalar operands")
	{
		f32x4 value{2.0f, -4.0f, 8.0f, -16.0f};

		value += 2.0f;
		requireSimdEquals(value, {4.0f, -2.0f, 10.0f, -14.0f});

		value -= 2.0f;
		requireSimdEquals(value, {2.0f, -4.0f, 8.0f, -16.0f});

		value *= -0.5f;
		requireSimdEquals(value, {-1.0f, 2.0f, -4.0f, 8.0f});

		value /= -0.5f;
		requireSimdEquals(value, {2.0f, -4.0f, 8.0f, -16.0f});
	}

	SECTION("construction and free helpers")
	{
		constexpr std::array<float, 4> data{5.0f, 6.0f, 7.0f, 8.0f};
		std::array<float, 4> stored{};

		const f32x4 splatted{3};
		const f32x4 fromPointer{data.data()};
		const f32x4 fromSpan{std::span<const float, 4>{data}};
		const f32x4 fromArgs{5.0f, 6.0f, 7.0, 8.0};
		const fixed_simd<float, 4> fixedLoaded = load<float, fixed_abi<4>>(data.data());
		const fixed_simd<float, 4> fixedSplatted = splat<float, fixed_abi<4>>(9.0f);
		const f32x4 roundTripped{fromArgs.native()};

		requireSimdEquals(splatted, {3.0f, 3.0f, 3.0f, 3.0f});
		requireSimdEquals(fromPointer, data);
		requireSimdEquals(fromSpan, data);
		requireSimdEquals(fromArgs, data);
		requireSimdEquals(fixedLoaded, data);
		requireSimdEquals(fixedSplatted, {9.0f, 9.0f, 9.0f, 9.0f});
		requireSimdEquals(roundTripped, data);

		fromArgs.store(stored.data());
		requireArrayEquals(stored, data);
	}
}

TEST_CASE("f32x reductions and elementwise helpers", "[simd]")
{
	const f32x4 a{4.0f, -2.0f, 9.0f, -16.0f};
	const f32x4 b{3.0f, -4.0f, 12.0f, -8.0f};

	REQUIRE(hsum(a) == Catch::Approx(-5.0f));
	REQUIRE(hmin(a) == Catch::Approx(-16.0f));
	REQUIRE(hmax(a) == Catch::Approx(9.0f));

	requireSimdEquals(min(a, b), {3.0f, -4.0f, 9.0f, -16.0f});
	requireSimdEquals(max(a, b), {4.0f, -2.0f, 12.0f, -8.0f});

	if constexpr (requires(f32x4 value) { neg(value); })
		requireSimdEquals(neg(a), {-4.0f, 2.0f, -9.0f, 16.0f});

	if constexpr (requires(f32x4 value) { abs(value); })
		requireSimdEquals(abs(a), {4.0f, 2.0f, 9.0f, 16.0f});

	if constexpr (requires(f32x4 value) { sqrt(value); })
		requireSimdEquals(sqrt(abs(a)), {2.0f, std::sqrt(2.0f), 3.0f, 4.0f});
}

TEST_CASE("f64x and integer simd paths stay wired up", "[simd]")
{
	SECTION("double precision arithmetic and reductions")
	{
		const f64x2 a{2.5, -9.0};
		const f64x2 b{4.0, 3.0};

		requireSimdEquals(a + b, {6.5, -6.0});
		requireSimdEquals(a / b, {0.625, -3.0});
#if defined(__SSE4_1__) || defined(__ARM_NEON)
		REQUIRE(hsum(a) == Catch::Approx(-6.5));
		REQUIRE(hmin(a) == Catch::Approx(-9.0));
		REQUIRE(hmax(a) == Catch::Approx(2.5));
#endif
	}

	SECTION("signed integer arithmetic and reductions")
	{
#if defined(__SSE4_1__) || defined(__ARM_NEON)
		const i32x4 a{10, -20, 30, -40};
		const i32x4 b{-1, 2, -3, 4};

		requireSimdEquals(a + b, {9, -18, 27, -36});
		requireSimdEquals(a - b, {11, -22, 33, -44});
		requireSimdEquals(min(a, b), {-1, -20, -3, -40});
		requireSimdEquals(max(a, b), {10, 2, 30, 4});
		REQUIRE(hsum(a) == -20);
		REQUIRE(hmin(a) == -40);
		REQUIRE(hmax(a) == 30);
#endif
	}

	SECTION("unsigned integer arithmetic and reductions")
	{
#if defined(__SSE4_1__) || defined(__ARM_NEON)
		const u32x4 a{2U, 4U, 6U, 8U};
		const u32x4 b{1U, 5U, 3U, 9U};

		requireSimdEquals(a + b, {3U, 9U, 9U, 17U});
		requireSimdEquals(a - u32x4{1U, 1U, 1U, 1U}, {1U, 3U, 5U, 7U});
		requireSimdEquals(min(a, b), {1U, 4U, 3U, 8U});
		requireSimdEquals(max(a, b), {2U, 5U, 6U, 9U});
		REQUIRE(hsum(a) == 20U);
		REQUIRE(hmin(a) == 2U);
		REQUIRE(hmax(a) == 8U);
#endif
	}
}
