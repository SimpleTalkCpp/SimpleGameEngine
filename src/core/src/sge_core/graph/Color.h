#pragma once

#include <sge_core/math/Tuple4.h>

namespace sge {

template<class T>
struct ColorR {
	using ElementType = T;
	static const size_t kElementCount = 1;

	union {
		struct { T r; };
		T data[kElementCount];
	};

	ColorR(const T& r_)
		: r(r_) {}
};

template<class T>
struct ColorRG {
	using ElementType = T;
	static const size_t kElementCount = 2;

	union {
		struct { T r, g; };
		T data[kElementCount];
	};

	ColorRG(const T& r_, const T& g_)
		: r(r_), g(g_) {}

};

template<class T>
struct ColorRGB {
	using ElementType = T;
	static const size_t kElementCount = 3;

	union {
		struct { T r, g, b; };
		T data[kElementCount];
	};

	ColorRGB(const T& r_, const T& g_, const T& b_)
		: r(r_), g(g_), b(b_) {}

};

template<class T>
struct ColorRGBA {
	using ElementType = T;
	static const size_t kElementCount = 4;

	union {
		struct { T r, g, b, a; };
		T data[kElementCount];
	};

	ColorRGBA(const T& r_, const T& g_, const T& b_, const T& a_)
		: r(r_), g(g_), b(b_), a(a_) {}

	ColorRGB<T> rgb() const { return ColorRGB(r,g,b); }

	Tuple4<T>	toTuple() const { return Tuple4<T>(r,g,b,a); }
	operator Tuple4<T>() const { return toTuple(); }

};

using ColorRGBAf = ColorRGBA<float>;
using ColorRGBAb = ColorRGBA<u8>;

using Color4f = ColorRGBAf;
using Color4b = ColorRGBAb;

}