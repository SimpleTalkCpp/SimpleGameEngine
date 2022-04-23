#pragma once

namespace sge {

namespace Math {
	template<class T> constexpr T max(const T& a, const T& b) { return a > b ? a : b; }
	template<class T> constexpr T min(const T& a, const T& b) { return a < b ? a : b; }

	template<class T> constexpr T ByteToK(const T& v) { return v / 1024; }
	template<class T> constexpr T ByteToM(const T& v) { return v / (1024 * 1024); }
	template<class T> constexpr T ByteToG(const T& v) { return v / (1024 * 1024 * 1024); }

	struct _Helper {
		template<class T>
		static constexpr T alignTo_uint(T n, T a) {
			static_assert(std::is_unsigned<T>::value, "");
			T r = n % a;
			return r ? (n + a - r) : n;
		}
	};

	constexpr size_t alignTo(size_t n, size_t a) { return _Helper::alignTo_uint(n, a); }
}

}