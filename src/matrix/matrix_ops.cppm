/*
 * SPDX-FileCopyrightText: 2026 Marcus Gigandet
 *
 * SPDX-License-Identifier: Apache-2.0
 */

module;
#include "typedefs.hpp"
#include <cstddef>
#include <type_traits>
export module mag:matrix_ops;

import :matrix;

export namespace MAG_NAMESPACE
{
	template <Numeric T, Numeric U, std::size_t R, std::size_t C>
	constexpr bool operator==(const Mat<T, C, R>& lhs, const Mat<U, C, R>& rhs) noexcept
	{
		for (std::size_t c = 0; c < C; ++c)
		{
			for (std::size_t r = 0; r < R; ++r)
			{
				if (lhs(c, r) != rhs(c, r))
				{
					return false;
				}
			}
		}
		return true;
	}

	template <Numeric T, Numeric U, std::size_t R, std::size_t C>
	constexpr bool operator!=(const Mat<T, C, R>& lhs, const Mat<U, C, R>& rhs) noexcept
	{
		return !(lhs == rhs);
	}

	template <Numeric T, Numeric U, std::size_t R, std::size_t C>
	constexpr auto operator+(const Mat<T, C, R>& a, U val) noexcept
	{
		Mat<std::common_type_t<T, U>, R, C> ret;
		for (std::size_t c = 0; c < C; ++c)
		{
			for (std::size_t r = 0; r < R; ++r)
			{
				ret(c, r) = a(c, r) + static_cast<T>(val);
			}
		}
		return ret;
	}

	template <Numeric T, Numeric U, std::size_t R, std::size_t C>
	constexpr auto operator-(const Mat<T, C, R>& a, U val) noexcept
	{
		Mat<std::common_type_t<T, U>, R, C> ret;
		for (std::size_t c = 0; c < C; ++c)
		{
			for (std::size_t r = 0; r < R; ++r)
			{
				ret(c, r) = a(c, r) - static_cast<T>(val);
			}
		}
		return ret;
	}

	template <Numeric T, Numeric U, std::size_t R, std::size_t C>
	constexpr auto operator*(const Mat<T, C, R>& a, U val) noexcept
	{
		Mat<std::common_type_t<T, U>, R, C> ret;
		for (std::size_t c = 0; c < C; ++c)
		{
			for (std::size_t r = 0; r < R; ++r)
			{
				ret(c, r) = a(c, r) * static_cast<T>(val);
			}
		}
		return ret;
	}

	template <Numeric T, Numeric U, std::size_t R, std::size_t C>
	constexpr auto operator/(const Mat<T, C, R>& a, U val) noexcept
	{
		Mat<std::common_type_t<T, U>, R, C> ret;
		for (std::size_t c = 0; c < C; ++c)
		{
			for (std::size_t r = 0; r < R; ++r)
			{
				ret(c, r) = a(c, r) / static_cast<T>(val);
			}
		}
		return ret;
	}

	template <Numeric T, Numeric U, std::size_t R, std::size_t C>
	constexpr auto operator+(const Mat<T, C, R>& lhs, const Mat<U, C, R>& rhs) noexcept
	{
		Mat<std::common_type_t<T, U>, R, C> ret;
		for (std::size_t c = 0; c < C; ++c)
		{
			for (std::size_t r = 0; r < R; ++r)
			{
				ret(c, r) = lhs(c, r) + static_cast<T>(rhs(c, r));
			}
		}
		return ret;
	}

	template <Numeric T, Numeric U, std::size_t R, std::size_t C>
	constexpr auto operator-(const Mat<T, C, R>& lhs, const Mat<U, C, R>& rhs) noexcept
	{
		Mat<std::common_type_t<T, U>, R, C> ret;
		for (std::size_t c = 0; c < C; ++c)
		{
			for (std::size_t r = 0; r < R; ++r)
			{
				ret(c, r) = lhs(c, r) - static_cast<T>(rhs(c, r));
			}
		}
		return ret;
	}

	template <Numeric T, Numeric U, std::size_t R, std::size_t C, std::size_t K>
	constexpr auto operator*(const Mat<T, R, K>& lhs, const Mat<U, K, C>& rhs) noexcept
	{
		Mat<std::common_type_t<T, U>, R, C> result;
		for (std::size_t c = 0; c < C; ++c)
		{
			for (std::size_t r = 0; r < R; ++r)
			{
				for (std::size_t k = 0; k < K; ++k)
				{
					result(c, r) += lhs(c, k) * rhs(k, r);
				}
			}
		}
		return result;
	}
} // namespace MAG_NAMESPACE
