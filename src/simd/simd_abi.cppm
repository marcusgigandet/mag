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
#include <cstddef>
#include <type_traits>

export module mag:simd_abi;

import :concepts;

namespace mag::simd
{
	export struct scalar_abi
	{
	};
	export struct native_abi
	{
	};
	export template <size_t N>
	struct fixed_abi
	{
		static constexpr size_t lanes = N;
	};

	consteval size_t nativeRegisterBytes() noexcept
	{
#ifdef MAG_NEON_SIMD
		return 16;
#elif MAG_X86_SIMD
		return 16;
#else
		return 0;
#endif
	}

	template <Numeric T>
	consteval size_t defaultLanes() noexcept
	{
		constexpr size_t bytes = nativeRegisterBytes();
		if constexpr (bytes == 0)
		{
			return 1;
		}
		else
		{
			constexpr size_t lanes = bytes / sizeof(T);
			return lanes == 0 ? 1 : lanes;
		}
	}

	template <Numeric T, typename Abi>
	struct abi_lanes;

	template <Numeric T>
	struct abi_lanes<T, scalar_abi> : std::integral_constant<size_t, 1>
	{
	};

	template <Numeric T>
	struct abi_lanes<T, native_abi> : std::integral_constant<size_t, defaultLanes<T>()>
	{
	};

	template <Numeric T, size_t N>
	struct abi_lanes<T, fixed_abi<N>> : std::integral_constant<size_t, N>
	{
	};

	export template <Numeric T, typename Abi = native_abi>
	inline constexpr size_t abi_lanes_v = abi_lanes<T, Abi>::value;
} // namespace mag::simd
