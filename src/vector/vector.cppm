module;
#include "typedefs.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <sstream>
#include <string>
export module mag:vector;

#ifdef MAG_ENABLE_SIMD
import mag.simd;
using namespace MAG_NAMESPACE::simd;
#endif

namespace MAG_NAMESPACE
{
	/**
	 * @brief Generic N-dimensional vector for numerical computations.
	 *
	 * @tparam T Numeric type of the vector elements.
	 * @tparam N Dimension of the vector.
	 */
	export template <Numeric T, std::size_t N>
	struct Vec;

	export template <Numeric T, std::size_t N>
	constexpr T length(const Vec<T, N>& v) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (supports_mul<T, N> && supports_reduction<T, N>)
		{
			Simd<T, N> va{v.v};
			return std::sqrt(hsum(va * va));
		}
#endif
		T ret{0};
		for (std::size_t i = 0; i < N; ++i)
		{
			ret += v[i] * v[i];
		}

		return std::sqrt(ret);
	}

	export template <Numeric T, std::size_t N>
	constexpr Vec<T, N> normalize(Vec<T, N> v)
	{
		float length{std::sqrt(v.x * v.x + v.y * v.y)};
		return (length > 0.0f) ? Vec<T, N>{v.x / length, v.y / length} : Vec<T, N>{0.0f, 0.0f};
	}

	export template <Numeric T, Numeric U, std::size_t N>
	constexpr auto dot(const Vec<T, N>& a, const Vec<U, N>& b) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (std::is_same_v<T, U> && supports_mul<T, N> && supports_reduction<T, N>)
		{
			Simd<T, N> va{a.v};
			Simd<T, N> vb{b.v};
			return hsum(va * vb);
		}
#endif
		using R = std::common_type_t<T, U>;
		R ret	= 0;
		for (std::size_t i = 0; i < N; ++i)
		{
			ret += a[i] * b[i];
		}
		return ret;
	}

	export template <Numeric T, std::size_t N>
	constexpr T distance(const Vec<T, N>& a, const Vec<T, N>& b) noexcept
	{
		return length(a - b);
	}

	export template <Numeric T, Numeric U, std::size_t N>
	constexpr Vec<T, N> lerp(const Vec<T, N>& a, const Vec<U, N>& b, T t) noexcept
	{
#ifdef MAG_ENABLE_SIMD
		if constexpr (
			std::is_same_v<T, U> && supports_splat<T, N> && supports_add<T, N> &&
			supports_sub<T, N> && supports_mul<T, N>)
		{
			Vec<T, N> r;

			Simd<T, N> va{a.v};
			Simd<T, N> vb{b.v};
			Simd<T, N> vt{t};

			auto vr{va + (vb - va) * vt};
			vr.store(r.v);
			return r;
		}
#endif
		return a + t * (b - a);
	}


	/**
	 * @brief Base class for N-dimensional vectors using the Curiously Recurring Template Pattern
	 * (CRTP). Provides common vector operations for derived vector classes.
	 *
	 * @tparam Derived The derived vector class (e.g., `Vec<T, N>`).
	 * @tparam T Numeric type of the vector elements.
	 * @tparam N Dimension of the vector.
	 */
	template <typename Derived, Numeric T, std::size_t N>
	struct IVec
	{
	private:
		/**
		 * @brief Returns a reference to the derived vector type.
		 *
		 * Provides access to the concrete vector implementation using CRTP.
		 * This allows the base class to call functions or access data defined
		 * in the derived class without virtual dispatch.
		 *
		 * @return Reference to the derived vector instance.
		 */
		constexpr Derived& derived() noexcept { return static_cast<Derived&>(*this); }

		/**
		 * @brief Returns a const reference to the derived vector type.
		 *
		 * Const-qualified version of derived(), enabling read-only access
		 * to the concrete vector implementation from const member functions.
		 *
		 * @return Const reference to the derived vector instance.
		 */
		constexpr const Derived& derived() const noexcept
		{
			return static_cast<const Derived&>(*this);
		}

	public:
		/********************/
		/* Iterator support */
		/********************/

		constexpr T* begin() noexcept { return &derived()[0]; }
		constexpr const T* begin() const noexcept { return &derived()[0]; }
		constexpr T* end() noexcept { return &derived()[0] + N; }
		constexpr const T* end() const noexcept { return &derived()[0] + N; }

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

		constexpr T& operator[](std::size_t i) noexcept { return derived().v[i]; }
		constexpr const T& operator[](std::size_t i) const noexcept { return derived().v[i]; }

		template <Numeric U>
		constexpr Vec<T, N>& operator+=(const Vec<U, N>& o) noexcept
		{
#ifdef MAG_ENABLE_SIMD
			if constexpr (std::is_same_v<T, U> && supports_add<T, N>)
			{
				Simd<T, N> va{derived().v};
				Simd<T, N> vb{o.v};

				auto vr{va + vb};
				vr.store(derived().v);

				return derived();
			}
#endif
			for (std::size_t i = 0; i < N; ++i)
			{
				derived()[i] += o[i];
			}
			return derived();
		}

		template <Numeric U>
		constexpr Vec<T, N>& operator-=(const Vec<U, N>& o) noexcept
		{
#ifdef MAG_ENABLE_SIMD
			if constexpr (std::is_same_v<T, U> && supports_sub<T, N>)
			{
				Simd<T, N> va{derived().v};
				Simd<T, N> vb{o.v};

				auto vr{va - vb};
				vr.store(derived().v);

				return derived();
			}
#endif
			for (std::size_t i = 0; i < N; ++i)
			{
				derived()[i] -= o[i];
			}
			return derived();
		}

		template <Numeric U>
		constexpr Vec<T, N>& operator*=(const Vec<U, N>& o) noexcept
		{
#ifdef MAG_ENABLE_SIMD
			if constexpr (std::is_same_v<T, U> && supports_mul<T, N>)
			{
				Simd<T, N> va{derived().v};
				Simd<T, N> vb{o.v};

				auto vr{va * vb};
				vr.store(derived().v);

				return derived();
			}
#endif
			for (std::size_t i = 0; i < N; ++i)
			{
				derived()[i] *= o[i];
			}
			return derived();
		}

		template <Numeric U>
		constexpr Vec<T, N>& operator/=(const Vec<U, N>& o) noexcept
		{
#ifdef MAG_ENABLE_SIMD
			if constexpr (std::is_same_v<T, U> && supports_div<T, N>)
			{
				Simd<T, N> va{derived().v};
				Simd<T, N> vb{o.v};

				auto vr{va / vb};
				vr.store(derived().v);

				return derived();
			}
#endif
			for (std::size_t i = 0; i < N; ++i)
			{
				derived()[i] /= o[i];
			}
			return derived();
		}

		template <Numeric U>
		constexpr auto operator+=(const U& s) noexcept
		{
#ifdef MAG_ENABLE_SIMD
			if constexpr (supports_add<T, N>)
			{
				Simd<T, N> va{derived().v};
				Simd<T, N> vb{s};

				auto vr{va + vb};
				vr.store(derived().v);

				return derived();
			}
#endif
			for (std::size_t i = 0; i < N; ++i)
			{
				derived()[i] += s;
			}
			return derived();
		}

		template <Numeric U>
		constexpr auto operator-=(const U& s) noexcept
		{
#ifdef MAG_ENABLE_SIMD
			if constexpr (supports_sub<T, N>)
			{
				Simd<T, N> va{derived().v};
				Simd<T, N> vb{s};

				auto vr{va - vb};
				vr.store(derived().v);

				return derived();
			}
#endif
			for (std::size_t i = 0; i < N; ++i)
			{
				derived()[i] -= s;
			}
			return derived();
		}

		template <Numeric U>
		constexpr auto operator*=(const U& s) noexcept
		{
#ifdef MAG_ENABLE_SIMD
			if constexpr (supports_mul<T, N>)
			{
				Simd<T, N> va{derived().v};
				Simd<T, N> vb{s};

				auto vr{va * vb};
				vr.store(derived().v);

				return derived();
			}
#endif
			for (std::size_t i = 0; i < N; ++i)
			{
				derived()[i] *= s;
			}
			return derived();
		}

		template <Numeric U>
		constexpr auto operator/=(U s) noexcept
		{
#ifdef MAG_ENABLE_SIMD
			if constexpr (supports_div<T, N>)
			{
				Simd<T, N> va{derived().v};
				Simd<T, N> vb{s};

				auto vr{va / vb};
				vr.store(derived().v);

				return derived();
			}
#endif
			for (std::size_t i = 0; i < N; ++i)
			{
				derived()[i] /= s;
			}
			return derived();
		}

		constexpr T length() const noexcept { return MAG_NAMESPACE::length(derived()); }

		constexpr Derived normalized() const noexcept
		{
			Derived ret = derived();
			T len{ret.length()};
			if (len > 0)
			{
				ret /= len;
			}
			return ret;
		}

		constexpr Derived clamped(T min, T max) const noexcept
		{
			Derived ret = derived();
			for (std::size_t i = 0; i < N; ++i)
			{
				ret[i] = std::clamp(derived()[i], min, max);
			}
			return ret;
		}

		template <Numeric U>
		constexpr auto dot(const Vec<U, N>& o) const noexcept
		{
			return MAG_NAMESPACE::dot(derived(), o);
		}

		/**
		 * @brief Returns the vector as a formatted string.
		 *
		 * The resulting string contains the vector dimension and all component values,
		 * e.g. `Vec3(1, 2, 3)`.
		 * @return A string in the form `VecN(...)`.
		 */
		[[nodiscard]] std::string toString() const noexcept
		{
			std::ostringstream oss;
			oss << "Vec" << N << "(";
			for (std::size_t i = 0; i < N; ++i)
			{
				if (i != N - 1)
				{
					oss << derived()[i] << ", ";
				}
				else
				{
					oss << derived()[i];
				}
			}
			oss << ")";
			return oss.str();
		}
	};

	template <Numeric T, std::size_t N>
	struct Vec : IVec<Vec<T, N>, T, N>
	{
		T v[N]{};

		constexpr Vec() = default;

		explicit constexpr Vec(T val)
		{
			for (std::size_t i = 0; i < N; ++i)
			{
				v[i] = val;
			}
		}
	};
}; // namespace MAG_NAMESPACE
