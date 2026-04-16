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
#include <iomanip>
#include <span>
#include <sstream>
export module mag:matrix;

import :concepts;
import :vector;


namespace mag
{
	/**
	 * @brief Generic CxR-dimensional matrix for numerical computations.
	 *
	 * @tparam T Numeric type of the matrix elements.
	 * @tparam C Number of columns in the matrix.
	 * @tparam R Number of rows in the matrix.
	 */
	export template <Numeric T, size_t C, size_t R>
	struct Mat;

	/**
	 * @brief Base class for CxR-dimensional matrices using the Curiously Recurring Template
	 * Pattern (CRTP). Provides common matrix operations for derived matrix classes.
	 *
	 * @tparam Derived The derived matrix class (e.g., `Mat<T, N>`).
	 * @tparam T Numeric type of the vector elements.
	 * @tparam C Number of columns in the matrix.
	 * @tparam R Number of rows in the matrix.
	 */
	template <typename Derived, Numeric T, size_t C, size_t R>
	struct IMat
	{
	private:
		/**
		 * @brief Returns a reference to the derived matrix type.
		 *
		 * Provides access to the concrete matrix implementation using CRTP.
		 * This allows the base class to call functions or access data defined
		 * in the derived class without virtual dispatch.
		 *
		 * @return Reference to the derived matrix instance.
		 */
		constexpr Derived& derived() noexcept { return static_cast<Derived&>(*this); }

		/**
		 * @brief Returns a reference to the derived matrix type.
		 *
		 * Provides access to the concrete matrix implementation using CRTP.
		 * This allows the base class to call functions or access data defined
		 * in the derived class without virtual dispatch.
		 *
		 * @return Const reference to the derived matrix instance.
		 */
		constexpr const Derived& derived() const noexcept
		{
			return static_cast<const Derived&>(*this);
		}

	public:
		/********************/
		/* Iterator support */
		/********************/

		constexpr T* begin() noexcept { return &derived().m[0][0]; }

		constexpr const T* begin() const noexcept { return &derived().m[0][0]; }

		constexpr T* end() noexcept { return &derived().m[0][0] + C * R; }

		constexpr const T* end() const noexcept { return &derived().m[0][0] + C * R; }

		/****************************/
		/* Reverse iterator support */
		/****************************/

		constexpr std::reverse_iterator<T*> rbegin() noexcept
		{
			return std::reverse_iterator<T*>(end());
		}

		constexpr std::reverse_iterator<const T*> rbegin() const noexcept
		{
			return std::reverse_iterator<const T*>(end());
		}

		constexpr std::reverse_iterator<T*> rend() noexcept
		{
			return std::reverse_iterator<T*>(begin());
		}

		constexpr std::reverse_iterator<const T*> rend() const noexcept
		{
			return std::reverse_iterator<const T*>(begin());
		}

		/**************************/
		/* Const iterator support */
		/**************************/

		constexpr const T* cbegin() const noexcept { return begin(); }

		constexpr const T* cend() const noexcept { return end(); }

		constexpr std::reverse_iterator<const T*> crbegin() const noexcept { return rbegin(); }

		constexpr std::reverse_iterator<const T*> crend() const noexcept { return rend(); }

		constexpr std::span<T, R> operator[](size_t i) noexcept
		{
			return std::span<T, R>(derived().m[i]);
		}

		constexpr std::span<const T, R> operator[](size_t i) const noexcept
		{
			return derived().m[i];
		}

		constexpr T& operator()(size_t c, size_t r) noexcept { return derived().m[c][r]; }

		constexpr const T& operator()(size_t c, size_t r) const noexcept
		{
			return derived().m[c][r];
		}

		constexpr T* data() noexcept { return &derived().m[0][0]; }
		constexpr const T* data() const noexcept { return &derived().m[0][0]; }

		template <Numeric U>
		constexpr Derived& operator+=(U val) noexcept
		{
			for (size_t c = 0; c < C; ++c)
				for (size_t r = 0; r < R; ++r)
					derived()(c, r) += val;
			return derived();
		}

		template <Numeric U>
		constexpr Derived& operator-=(U val) noexcept
		{
			for (size_t c = 0; c < C; ++c)
				for (size_t r = 0; r < R; ++r)
					derived()(c, r) -= val;
			return derived();
		}

		template <Numeric U>
		constexpr Derived& operator*=(U val) noexcept
		{
			for (size_t c = 0; c < C; ++c)
				for (size_t r = 0; r < R; ++r)
					derived()(c, r) *= val;
			return derived();
		}

		template <Numeric U>
		constexpr Derived& operator/=(U val) noexcept
		{
			for (size_t c = 0; c < C; ++c)
				for (size_t r = 0; r < R; ++r)
					derived()(c, r) /= val;
			return derived();
		}

		template <Numeric U>
		constexpr Derived& operator+=(Mat<U, C, R> o) noexcept
		{
			for (size_t c = 0; c < C; ++c)
				for (size_t r = 0; r < R; ++r)
					derived()(c, r) += o[c][r];
			return derived();
		}

		template <Numeric U>
		constexpr Derived& operator-=(Mat<U, C, R> o) noexcept
		{
			for (size_t c = 0; c < C; ++c)
				for (size_t r = 0; r < R; ++r)
					derived()(c, r) -= o[c][r];
			return derived();
		}

		template <Numeric U>
		constexpr Derived& operator*=(Mat<U, C, R> o) noexcept
		{
			for (size_t c = 0; c < C; ++c)
				for (size_t r = 0; r < R; ++r)
					derived()(c, r) *= o[c][r];
			return derived();
		}

		constexpr Mat<T, R, C> transpose() const noexcept
		{
			Mat<T, R, C> result{};
			for (size_t c = 0; c < C; ++c)
				for (size_t r = 0; r < R; ++r)
					result(r, c) = derived()[c][r];
			return result;
		}

		static constexpr Derived diagonal(const T val) noexcept
		{
			Derived result{};
			const size_t limit = (R < C ? R : C);
			for (size_t i = 0; i < limit; ++i)
				result(i, i) = val;
			return result;
		}

		static constexpr Derived diagonal(const Vec<T, R> diagVals) noexcept
		{
			Derived result{};
			const size_t limit = (R < C) ? R : C;
			for (size_t i = 0; i < limit; ++i)
				result(i, i) = diagVals[i];
			return result;
		}

		[[nodiscard]] std::string toString() const noexcept
		{
			std::ostringstream oss;
			const auto m{derived()};

			oss << std::fixed << std::setprecision(6);
			oss << "Mat" << R << "x" << C << "(\n";
			for (size_t r = 0; r < R; ++r)
			{
				oss << "\t[";
				for (size_t c = 0; c < C; ++c)
				{
					oss << m[c][r];
					if (c != C - 1)
						oss << ", ";
				}
				oss << "]";
				if (r < R - 1)
					oss << ", ";
				oss << "\n";
			}
			oss << ")";
			return oss.str();
		}
	};

	export template <Numeric T, size_t C, size_t R>
	struct Mat : IMat<Mat<T, C, R>, T, C, R>
	{
		T v[C][R];

		constexpr Mat() noexcept
		{
			for (size_t c = 0; c < C; ++c)
				for (size_t r = 0; r < R; ++r)
					v(c, r) = static_cast<T>(0);
		}

		template <Numeric U>
		constexpr explicit Mat(U val) noexcept
		{
			for (size_t c = 0; c < C; ++c)
				for (size_t r = 0; r < R; ++r)
					v(c, r) = static_cast<T>(val);
		}
	};
} // namespace mag
