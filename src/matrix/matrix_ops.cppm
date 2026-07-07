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
#include "typedefs.hpp"
#include <cstddef>
#include <type_traits>
export module mag:matrix_ops;

import :matrix;

export namespace mag
{
	template <Numeric T, Numeric U, size_t R, size_t C>
	constexpr bool operator==(const Mat<T, C, R>& lhs, const Mat<U, C, R>& rhs) noexcept
	{
		for (size_t c = 0; c < C; ++c)
			for (size_t r = 0; r < R; ++r)
				if (lhs(c, r) != rhs(c, r))
					return false;
		return true;
	}

	template <Numeric T, Numeric U, size_t R, size_t C>
	constexpr bool operator!=(const Mat<T, C, R>& lhs, const Mat<U, C, R>& rhs) noexcept
	{
		return !(lhs == rhs);
	}

	template <Numeric T, Numeric U, size_t R, size_t C>
	constexpr auto operator+(const Mat<T, C, R>& a, U val) noexcept
	{
		Mat<std::common_type_t<T, U>, R, C> ret;
		for (size_t c = 0; c < C; ++c)
			for (size_t r = 0; r < R; ++r)
				ret(c, r) = a(c, r) + static_cast<T>(val);
		return ret;
	}

	template <Numeric T, Numeric U, size_t R, size_t C>
	constexpr auto operator-(const Mat<T, C, R>& a, U val) noexcept
	{
		Mat<std::common_type_t<T, U>, R, C> ret;
		for (size_t c = 0; c < C; ++c)
			for (size_t r = 0; r < R; ++r)
				ret(c, r) = a(c, r) - static_cast<T>(val);
		return ret;
	}

	template <Numeric T, Numeric U, size_t R, size_t C>
	constexpr auto operator*(const Mat<T, C, R>& a, U val) noexcept
	{
		Mat<std::common_type_t<T, U>, R, C> ret;
		for (size_t c = 0; c < C; ++c)
			for (size_t r = 0; r < R; ++r)
				ret(c, r) = a(c, r) * static_cast<T>(val);
		return ret;
	}

	template <Numeric T, Numeric U, size_t R, size_t C>
	constexpr auto operator/(const Mat<T, C, R>& a, U val) noexcept
	{
		Mat<std::common_type_t<T, U>, R, C> ret;
		for (size_t c = 0; c < C; ++c)
			for (size_t r = 0; r < R; ++r)
				ret(c, r) = a(c, r) / static_cast<T>(val);
		return ret;
	}

	template <Numeric T, Numeric U, size_t R, size_t C>
	constexpr auto operator+(const Mat<T, C, R>& lhs, const Mat<U, C, R>& rhs) noexcept
	{
		Mat<std::common_type_t<T, U>, R, C> ret;
		for (size_t c = 0; c < C; ++c)
			for (size_t r = 0; r < R; ++r)
				ret(c, r) = lhs(c, r) + static_cast<T>(rhs(c, r));
		return ret;
	}

	template <Numeric T, Numeric U, size_t R, size_t C>
	constexpr auto operator-(const Mat<T, C, R>& lhs, const Mat<U, C, R>& rhs) noexcept
	{
		Mat<std::common_type_t<T, U>, R, C> ret;
		for (size_t c = 0; c < C; ++c)
			for (size_t r = 0; r < R; ++r)
				ret(c, r) = lhs(c, r) - static_cast<T>(rhs(c, r));
		return ret;
	}

	template <Numeric T, Numeric U, size_t R, size_t C, size_t K>
	constexpr auto operator*(const Mat<T, R, K>& lhs, const Mat<U, K, C>& rhs) noexcept
	{
		Mat<std::common_type_t<T, U>, R, C> result{};
		for (size_t c = 0; c < C; ++c)
			for (size_t r = 0; r < R; ++r)
				for (size_t k = 0; k < K; ++k)
					result(c, r) += lhs(c, k) * rhs(k, r);
		return result;
	}
} // namespace mag
