#pragma once

namespace sge {

template<class T>
struct ColorR {
	using ElementType = T;
	static const size_t kElementCount = 1;

	union {
		struct { T r; };
		T data[kElementCount];
	};
};

template<class T>
struct ColorRG {
	using ElementType = T;
	static const size_t kElementCount = 2;

	union {
		struct { T r, g; };
		T data[kElementCount];
	};
};

template<class T>
struct ColorRGB {
	using ElementType = T;
	static const size_t kElementCount = 3;

	union {
		struct { T r, g, b; };
		T data[kElementCount];
	};
};

template<class T>
struct ColorRGBA {
	using ElementType = T;
	static const size_t kElementCount = 4;

	ColorRGB<T> rgb() const { return ColorRGB(r,g,b); }

	union {
		struct { T r, g, b, a; };
		T data[kElementCount];
	};
};

using ColorRGBAf = ColorRGBA<float>;
using ColorRGBAb = ColorRGBA<u8>;

using Color4f = ColorRGBAf;
using Color4b = ColorRGBAb;

}