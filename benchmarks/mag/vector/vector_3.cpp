#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <glm/glm.hpp>
#include <vector>

import mag;
using namespace mag;

TEST_CASE("Benchmark basic 3D vector workloads", "[benchmark]")
{
	static constexpr std::size_t N = 1'000'000;

	std::vector<Vec<float, 3>> a(N, {1.0f, 2.0f, 3.0f});
	std::vector<Vec<float, 3>> b(N, {4.0f, 5.0f, 6.0f});

	std::vector<glm::vec3> glm_a(N, {1.0f, 2.0f, 3.0f});
	std::vector<glm::vec3> glm_b(N, {4.0f, 5.0f, 6.0f});

	BENCHMARK("3D vector add")
	{
		std::vector<Vec<float, 3>> c;
		c.reserve(N);

		for (std::size_t i = 0; i < N; ++i)
		{
			const auto& va = a[i];
			const auto& vb = b[i];

			c.emplace_back(va.x + vb.x, va.y + vb.y, va.z + vb.z);
		}

		return c;
	};

	BENCHMARK("3D glm vector add")
	{
		std::vector<glm::vec3> c;
		c.reserve(N);

		for (std::size_t i = 0; i < N; ++i)
		{
			const auto& va = glm_a[i];
			const auto& vb = glm_b[i];

			c.emplace_back(va.x + vb.x, va.y + vb.y, va.z + vb.z);
		}

		return c;
	};
}
