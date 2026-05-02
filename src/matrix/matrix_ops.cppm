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
export module mag:matrix_ops;

import :concepts;
import :matrix;

export namespace mag
{
	template <Numeric T, Numeric U, size_t R, size_t C>
	constexpr bool operator==(const Mat<T, C, R>& a, const Mat<U, C, R>& b) noexcept
	{
		for (size_t c = 0; c < C; ++c)
			for (size_t r = 0; r < R; ++r)
				if (a(c, r) != b(c, r))
					return false;
		return true;
	}

	template <Numeric T, Numeric U, size_t R, size_t C>
	constexpr bool operator!=(const Mat<T, C, R>& a, const Mat<U, C, R>& b) noexcept
	{
		return !(a == b);
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
	constexpr auto operator+(const Mat<T, C, R>& a, const Mat<U, C, R>& b) noexcept
	{
		Mat<std::common_type_t<T, U>, R, C> ret;
		for (size_t c = 0; c < C; ++c)
			for (size_t r = 0; r < R; ++r)
				ret(c, r) = a(c, r) + static_cast<T>(b(c, r));
		return ret;
	}

	template <Numeric T, Numeric U, size_t R, size_t C>
	constexpr auto operator-(const Mat<T, C, R>& a, const Mat<U, C, R>& b) noexcept
	{
		Mat<std::common_type_t<T, U>, R, C> ret;
		for (size_t c = 0; c < C; ++c)
			for (size_t r = 0; r < R; ++r)
				ret(c, r) = a(c, r) - static_cast<T>(b(c, r));
		return ret;
	}

	template <Numeric T, Numeric U, size_t R, size_t C, size_t K>
	constexpr auto operator*(const Mat<T, R, K>& a, const Mat<U, K, C>& b) noexcept
	{
		Mat<std::common_type_t<T, U>, R, C> result{};
		for (size_t c = 0; c < C; ++c)
			for (size_t r = 0; r < R; ++r)
				for (size_t k = 0; k < K; ++k)
					result(c, r) += a(c, k) * b(k, r);
		return result;
	}
} // namespace mag
