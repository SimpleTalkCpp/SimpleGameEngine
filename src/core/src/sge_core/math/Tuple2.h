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

	SGE_INLINE			T& operator[](int i)		{ return data[i]; }
	SGE_INLINE	const	T& operator[](int i) const	{ return data[i]; }

	SGE_INLINE Tuple2() = default;
	SGE_INLINE Tuple2(const T& x_, const T& y_) { set(x_, y_); }

	SGE_INLINE void set(const Tuple2<T>& v) { *this = v; }
	SGE_INLINE void set(const T& x_, const T& y_) {
		x = x_; y = y_;
	}

	SGE_INLINE bool setAll(const T& v) { set(v,v); }
};

using Tuple2f = Tuple2<float>;
using Tuple2d = Tuple2<double>;

}