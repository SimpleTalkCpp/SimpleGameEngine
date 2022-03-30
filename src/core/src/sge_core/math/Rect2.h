#pragma once

#include "Vec2.h"

namespace sge {

template<class T>
class Rect2 {
public:
	using ElementType = T;
	static const size_t kElementCount = 4;

	using Vec2 = Vec2<T>;

	union {
		struct { T x, y, w, h;};
		struct { Vec2 pos, size; };
		T data[kElementCount];
	};
};

using Rect2i = Rect2<int>;
using Rect2f = Rect2<float>;

}