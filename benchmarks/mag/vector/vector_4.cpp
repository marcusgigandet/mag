#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <glm/glm.hpp>

#include <vector>

import mag;

using namespace mag;

namespace
{
	constexpr std::size_t N{1'000'000};
	constexpr std::size_t ITERATIONS{32};

	std::vector<Vec4f> a(N);
	std::vector<Vec4f> b(N);

	std::vector<glm::vec4> glm_a(N);
	std::vector<glm::vec4> glm_b(N);

	void initializeData()
	{
		for (std::size_t i = 0; i < N; ++i)
		{
			const float x{static_cast<float>(i) * 0.001f};

			a[i] = Vec4f(x, x + 1.0f, x + 2.0f, x + 3.0f);
			b[i] = Vec4f(x + 4.0f, x + 5.0f, x + 6.0f, x + 7.0f);

			glm_a[i] = glm::vec4(x, x + 1.0f, x + 2.0f, x + 3.0f);
			glm_b[i] = glm::vec4(x + 4.0f, x + 5.0f, x + 6.0f, x + 7.0f);
		}
	}

	float sink{0.0f};
} // namespace

TEST_CASE("Benchmark - Vec4 workloads", "[benchmark]")
{
	initializeData();

	BENCHMARK("mag - Vec4 workload 1")
	{
		Vec<float, 4> accum{0.0f};

		for (std::size_t iter = 0; iter < ITERATIONS; ++iter)
		{
			for (std::size_t i = 0; i < N; ++i)
			{
				auto v{a[i] * 0.5f + b[i] * 1.5f};
				const auto len{v.length()};

				v /= len;
				accum += v * 0.25f;
			}
		}

		sink = accum.x + accum.y + accum.z + accum.w;

		return sink;
	};

	BENCHMARK("glm - Vec4 workload 1")
	{
		glm::vec4 accum{0.0f};

		for (std::size_t iter = 0; iter < ITERATIONS; ++iter)
		{
			for (std::size_t i = 0; i < N; ++i)
			{
				auto v{glm_a[i] * 0.5f + glm_b[i] * 1.5f};
				const float len{glm::length(v)};

				v /= len;
				accum += v * 0.25f;
			}
		}

		sink = accum.x + accum.y + accum.z + accum.w;

		return sink;
	};
}
