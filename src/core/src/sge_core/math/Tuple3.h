#pragma once

namespace sge {

template<class T>
struct Tuple3 {
	using ElementType = T;
	static const size_t kElementCount = 3;

	union {
		struct { T x, y, z; };
		T data[kElementCount];
	};

	Tuple3(const T& x_, const T& y_, const T& z_)
		: x(x_) , y(y_), z(z_) {}
};

using Tuple3f = Tuple3<float>;
using Tuple3d = Tuple3<double>;

}