/*
 * SPDX-FileCopyrightText: 2026 Marcus Gigandet
 *
 * SPDX-License-Identifier: Apache-2.0
 */

module;
#include "typedefs.hpp"
#include <concepts>
#include <cstddef>
export module mag.simd:concepts;

import :ops;

export namespace MAG_NAMESPACE::simd
{
	template <typename T, std::size_t N, simd_isa Isa = default_isa>
	concept supports_splat = requires(T s) {
		{ ops_impl<T, N, Isa>::splat(s) } -> std::same_as<typename ops_impl<T, N, Isa>::native_t>;
	};

	template <typename T, std::size_t N, simd_isa Isa = default_isa>
	concept supports_add = requires(
		typename ops_impl<T, N, Isa>::native_t a, typename ops_impl<T, N, Isa>::native_t b) {
		{ ops_impl<T, N, Isa>::add(a, b) } -> std::same_as<typename ops_impl<T, N, Isa>::native_t>;
	};

	template <typename T, std::size_t N, simd_isa Isa = default_isa>
	concept supports_sub = requires(
		typename ops_impl<T, N, Isa>::native_t a, typename ops_impl<T, N, Isa>::native_t b) {
		{ ops_impl<T, N, Isa>::sub(a, b) } -> std::same_as<typename ops_impl<T, N, Isa>::native_t>;
	};

	template <typename T, std::size_t N, simd_isa Isa = default_isa>
	concept supports_mul = requires(
		typename ops_impl<T, N, Isa>::native_t a, typename ops_impl<T, N, Isa>::native_t b) {
		{ ops_impl<T, N, Isa>::mul(a, b) } -> std::same_as<typename ops_impl<T, N, Isa>::native_t>;
	};

	template <typename T, std::size_t N, simd_isa Isa = default_isa>
	concept supports_div = requires(
		typename ops_impl<T, N, Isa>::native_t a, typename ops_impl<T, N, Isa>::native_t b) {
		{ ops_impl<T, N, Isa>::div(a, b) } -> std::same_as<typename ops_impl<T, N, Isa>::native_t>;
	};

	template <typename T, std::size_t N, simd_isa Isa = default_isa>
	concept supports_reduction = requires(typename ops_impl<T, N, Isa>::native_t v) {
		{ ops_impl<T, N, Isa>::hsum(v) } -> std::same_as<T>;
	};

	template <typename T, std::size_t N, simd_isa Isa = default_isa>
	concept supports_hmin = requires(
		typename ops_impl<T, N, Isa>::native_t a, typename ops_impl<T, N, Isa>::native_t b) {
		{ ops_impl<T, N, Isa>::hmin(a, b) } -> std::same_as<typename ops_impl<T, N, Isa>::native_t>;
	};

	template <typename T, std::size_t N, simd_isa Isa = default_isa>
	concept supports_hmax = requires(
		typename ops_impl<T, N, Isa>::native_t a, typename ops_impl<T, N, Isa>::native_t b) {
		{ ops_impl<T, N, Isa>::hmax(a, b) } -> std::same_as<typename ops_impl<T, N, Isa>::native_t>;
	};

	template <typename T, std::size_t N, simd_isa Isa = default_isa>
	concept supports_min = requires(
		typename ops_impl<T, N, Isa>::native_t a, typename ops_impl<T, N, Isa>::native_t b) {
		{ ops_impl<T, N, Isa>::min(a, b) } -> std::same_as<typename ops_impl<T, N, Isa>::native_t>;
	};

	template <typename T, std::size_t N, simd_isa Isa = default_isa>
	concept supports_max = requires(
		typename ops_impl<T, N, Isa>::native_t a, typename ops_impl<T, N, Isa>::native_t b) {
		{ ops_impl<T, N, Isa>::max(a, b) } -> std::same_as<typename ops_impl<T, N, Isa>::native_t>;
	};

	template <typename T, std::size_t N, simd_isa Isa = default_isa>
	concept supports_neg = requires(typename ops_impl<T, N, Isa>::native_t v) {
		{ ops_impl<T, N, Isa>::neg(v) } -> std::same_as<typename ops_impl<T, N, Isa>::native_t>;
	};

	template <typename T, std::size_t N, simd_isa Isa = default_isa>
	concept supports_abs = requires(typename ops_impl<T, N, Isa>::native_t v) {
		{ ops_impl<T, N, Isa>::abs(v) } -> std::same_as<typename ops_impl<T, N, Isa>::native_t>;
	};

	template <typename T, std::size_t N, simd_isa Isa = default_isa>
	concept supports_sqrt = requires(typename ops_impl<T, N, Isa>::native_t v) {
		{ ops_impl<T, N, Isa>::sqrt(v) } -> std::same_as<typename ops_impl<T, N, Isa>::native_t>;
	};
} // namespace MAG_NAMESPACE::simd
