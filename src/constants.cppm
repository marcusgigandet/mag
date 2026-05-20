/*
 * Copyright 2026 Marcus Gigandet
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

module;
#include <numbers>
export module mag:constants;

export namespace mag
{
	template <typename T>
	inline constexpr T pi = std::numbers::pi_v<T>;

	template <typename T>
	inline constexpr T e = std::numbers::e_v<T>;

	template <typename T>
	inline constexpr T phi = static_cast<T>(1.6180339887498948482045868343656381L);

	template <typename T>
	inline constexpr T half_pi = pi<T> / static_cast<T>(2);

	template <typename T>
	inline constexpr T deg_to_rad = pi<T> / static_cast<T>(180);

	template <typename T>
	inline constexpr T rad_to_deg = static_cast<T>(180) / pi<T>;
} // namespace mag
