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

	SGE_INLINE			T& operator[](int i)		{ return data[i]; }
	SGE_INLINE	const	T& operator[](int i) const	{ return data[i]; }

	SGE_INLINE Tuple3() = default;
	SGE_INLINE Tuple3(const T& x_, const T& y_, const T& z_) { set(x_, y_, z_); }

	SGE_INLINE void set(const Tuple3<T>& v) { *this = v; }
	SGE_INLINE void set(const T& x_, const T& y_, const T& z_) {
		x = x_; y = y_; z = z_;
	}

	SGE_INLINE bool setAll(const T& v) { set(v,v,v); }
};

using Tuple3f = Tuple3<float>;
using Tuple3d = Tuple3<double>;

}