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
#include <concepts>
#include <cstddef>
#include <cstdint>
export module mag:simd_ops;

export namespace mag::simd
{
	/**
	 * @brief SIMD backend operations interface.
	 *
	 * Provides the primitive SIMD operations for a given type and width.
	 *
	 * @note Must be specialized for each backend.
	 *
	 * @tparam T Scalar element type.
	 * @tparam N SIMD lane count.
	 */
	template <typename T, size_t N>
	struct ops
	{
		/**
		 * @brief Underlying SIMD register type for this backend.
		 */
		using simd_t = T;

		/**
		 * @brief Loads N elements from memory into a SIMD register.
		 *
		 * @param p Pointer to input array of at least N elements.
		 * @return SIMD register containing loaded values.
		 */
		static simd_t load(const T* p) noexcept = delete;


		/**
		 * @brief Stores SIMD register values back to memory.
		 *
		 * @param p Pointer to output array of at least N elements.
		 * @param v SIMD register to store.
		 */
		static void store(T* p, simd_t v) noexcept = delete;

		/**
		 * @brief Broadcasts a scalar value across all SIMD lanes.
		 *
		 * @param s Scalar value to replicate.
		 * @return SIMD register with all lanes set to s.
		 */
		static simd_t splat(T s) noexcept = delete;

		/**
		 * @brief Performs element-wise addition of two SIMD registers.
		 *
		 * @param a First operand.
		 * @param b Second operand.
		 * @return Result of a + b.
		 */
		static simd_t add(simd_t a, simd_t b) noexcept = delete;

		/**
		 * @brief Performs element-wise subtraction of two SIMD registers.
		 *
		 * @param a First operand.
		 * @param b Second operand.
		 * @return Result of a - b.
		 */
		static simd_t sub(simd_t a, simd_t b) noexcept = delete;

		/**
		 * @brief Performs element-wise multiplication of two SIMD registers.
		 *
		 * @param a First operand.
		 * @param b Second operand.
		 * @return Result of a * b.
		 */
		static simd_t mul(simd_t a, simd_t b) noexcept = delete;

		/**
		 * @brief Performs element-wise division of two SIMD registers.
		 *
		 * @param a First operand.
		 * @param b Second operand.
		 * @return Result of a / b.
		 */
		static simd_t div(simd_t a, simd_t b) noexcept = delete;

		/**
		 * @brief Reduces a SIMD register to a single scalar by summing all lanes.
		 *
		 * @param v SIMD register to reduce.
		 * @return Sum of all elements in v.
		 */
		static T horizontal_sum(simd_t v) noexcept = delete;
	};

	template <typename T, size_t N>
	concept loadable = requires(const T* p) {
		{ ops<T, N>::load(p) } -> std::same_as<typename ops<T, N>::simd_t>;
	};

	template <typename T, size_t N>
	concept storable = requires(T* p, typename ops<T, N>::simd_t v) {
		{ ops<T, N>::store(p, v) } -> std::same_as<void>;
	};

	template <typename T, size_t N>
	concept supports_splat = requires(T s) {
		{ ops<T, N>::splat(s) } -> std::same_as<typename ops<T, N>::simd_t>;
	};

	template <typename T, size_t N>
	concept supports_add = requires(typename ops<T, N>::simd_t a, typename ops<T, N>::simd_t b) {
		{ ops<T, N>::add(a, b) } -> std::same_as<typename ops<T, N>::simd_t>;
	};

	template <typename T, size_t N>
	concept supports_subtraction =
			requires(typename ops<T, N>::simd_t a, typename ops<T, N>::simd_t b) {
				{ ops<T, N>::sub(a, b) } -> std::same_as<typename ops<T, N>::simd_t>;
			};

	template <typename T, size_t N>
	concept supports_multiplication =
			requires(typename ops<T, N>::simd_t a, typename ops<T, N>::simd_t b) {
				{ ops<T, N>::mul(a, b) } -> std::same_as<typename ops<T, N>::simd_t>;
			};

	template <typename T, size_t N>
	concept supports_division =
			requires(typename ops<T, N>::simd_t a, typename ops<T, N>::simd_t b) {
				{ ops<T, N>::div(a, b) } -> std::same_as<typename ops<T, N>::simd_t>;
			};

	template <typename T, size_t N>
	concept supports_horizontal_sum = requires(typename ops<T, N>::simd_t v) {
		{ ops<T, N>::horizontal_sum(v) } -> std::same_as<T>;
	};

#ifdef MAG_ENABLE_SIMD_EXTENDED
	using i8x16 = ops<int8_t, 16>;
	using i16x8 = ops<int16_t, 8>;
	using i32x4 = ops<int32_t, 4>;
	using i64x2 = ops<int64_t, 2>;

	using u8x16 = ops<uint8_t, 16>;
	using u16x8 = ops<uint16_t, 8>;
	using u32x4 = ops<uint32_t, 4>;
	using u64x2 = ops<uint64_t, 2>;
#endif

	using f32x4 = ops<float, 4>;
	using f64x2 = ops<double, 2>;
} // namespace mag::simd
