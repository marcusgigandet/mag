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

export module mag:constants;

export namespace mag
{
	constexpr double PI = 3.14159265358979323846;
	constexpr float PI_F = 3.1415927f;

	constexpr double PI_2 = PI / 2;
	constexpr float PI_F_2 = PI_F / 2;

	constexpr double E = 2.71828182845904523536;
	constexpr float E_F = 2.7182817f;

	constexpr double PHI = 1.61803398874989484820;
	constexpr float PHI_F = 1.6180339f;

	constexpr double DEG_TO_RAD = PI / 180.0;
	constexpr float DEG_TO_RAD_F = PI_F / 180.0f;

	constexpr double RAD_TO_DEG = 180.0 / PI;
	constexpr float RAD_TO_DEG_F = 180.0f / PI_F;
} // namespace mag
