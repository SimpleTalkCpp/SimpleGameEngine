#pragma once

namespace sge {

template<class T>
struct Tuple2 {
	using ElementType = T;
	static const size_t kElementCount = 2;

	union {
		struct { T x, y; };
		T data[kElementCount];
	};

	Tuple2(const T& x_, const T& y_)
		: x(x_), y(y_) {}
};

using Tuple2f = Tuple2<float>;
using Tuple2d = Tuple2<double>;

}