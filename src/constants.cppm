/*
 * SPDX-FileCopyrightText: 2026 Marcus Gigandet
 *
 * SPDX-License-Identifier: Apache-2.0
 */
module;
#include "typedefs.hpp"
#include <numbers>
export module mag:constants;

export namespace MAG_NAMESPACE
{
	template <typename T>
	inline constexpr T pi{std::numbers::pi_v<T>};

	template <typename T>
	inline constexpr T e{std::numbers::e_v<T>};

	template <typename T>
	inline constexpr T phi{static_cast<T>(std::numbers::phi_v<T>)};

	template <typename T>
	inline constexpr T half_pi{pi<T> / static_cast<T>(2)};

	template <typename T>
	inline constexpr T deg_to_rad{pi<T> / static_cast<T>(180)};

	template <typename T>
	inline constexpr T rad_to_deg{static_cast<T>(180) / pi<T>};
} // namespace MAG_NAMESPACE
