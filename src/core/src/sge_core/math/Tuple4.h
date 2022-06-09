#pragma once

namespace sge {

template<class T>
struct Tuple4 {
	using ElementType = T;
	static const size_t kElementCount = 4;

	union {
		struct { T x, y, z, w; };
		T data[kElementCount];
	};

	SGE_INLINE			T& operator[](int i)		{ return data[i]; }
	SGE_INLINE	const	T& operator[](int i) const	{ return data[i]; }

	SGE_INLINE Tuple4() = default;
	SGE_INLINE Tuple4(const T& x_, const T& y_, const T& z_, const T& w_) { set(x_, y_, z_, w_); }

	SGE_INLINE void set(const Tuple4<T>& v) { *this = v; }
	SGE_INLINE void set(const T& x_, const T& y_, const T& z_, const T& w_) {
		x = x_; y = y_; z = z_; w = w_;
	}

	SGE_INLINE bool setAll(const T& v) { set(v,v,v,v); }
};

using Tuple4f = Tuple4<float>;
using Tuple4d = Tuple4<double>;

}