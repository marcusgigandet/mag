/*
 * SPDX-FileCopyrightText: 2026 Marcus Gigandet
 *
 * SPDX-License-Identifier: Apache-2.0
 */

module;
#include <array>
#include <cmath>
export module mag:quat;

namespace mag
{
	export template <typename T>
	struct Quat
	{
		T x{};
		T y{};
		T z{};
		T w{1};


		/**
		 * @brief Returns the magnitutde of the quaternion.
		 */
		[[nodiscard]] T norm() const noexcept { return std::sqrt(norm2()); }

		[[nodiscard]] T norm2() const noexcept { return x * x + y * y + z * z + w * w; }
	};
} // namespace mag
