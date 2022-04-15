#pragma once

namespace sge {

namespace Math {
	template<class T> constexpr T ByteToK(const T& v) { return v / 1024; }
	template<class T> constexpr T ByteToM(const T& v) { return v / (1024 * 1024); }
	template<class T> constexpr T ByteToG(const T& v) { return v / (1024 * 1024 * 1024); }

}

}