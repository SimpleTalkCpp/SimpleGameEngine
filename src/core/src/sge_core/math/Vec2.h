#pragma once

#include <sge_core/base/Error.h>
#include "Tuple2.h"

namespace sge {

template<class T, class DATA = Tuple2<T> >
class Vec2 : public DATA {
public:
	using ElementType = T;
	static const size_t kElementCount = 2;

	using DATA::x;
	using DATA::y;

	Vec2() = default;
	Vec2(const T& x_, const T& y_) : DATA(x_, y_) {}
	Vec2(const Tuple2<T>& v) : DATA(v) {}

	Vec2 operator+(const Vec2& r) const { return Vec2(x + r.x, y + r.y); }
	Vec2 operator-(const Vec2& r) const { return Vec2(x - r.x, y - r.y); }
	Vec2 operator*(const Vec2& r) const { return Vec2(x * r.x, y * r.y); }
	Vec2 operator/(const Vec2& r) const { return Vec2(x / r.x, y / r.y); }

	Vec2 operator+(const T& s) const { return Vec2(x + s, y + s); }
	Vec2 operator-(const T& s) const { return Vec2(x - s, y - s); }
	Vec2 operator*(const T& s) const { return Vec2(x * s, y * s); }
	Vec2 operator/(const T& s) const { return Vec2(x / s, y / s); }

	SGE_INLINE void operator+= (const Vec2& r) { x += r.x; y += r.y; }
	SGE_INLINE void operator-= (const Vec2& r) { x -= r.x; y -= r.y; }
	SGE_INLINE void operator*= (const Vec2& r) { x *= r.x; y *= r.y; }
	SGE_INLINE void operator/= (const Vec2& r) { x /= r.x; y /= r.y; }

	SGE_INLINE void operator+= (const T& s) { x += s; y += s; }
	SGE_INLINE void operator-= (const T& s) { x -= s; y -= s; }
	SGE_INLINE void operator*= (const T& s) { x *= s; y *= s; }
	SGE_INLINE void operator/= (const T& s) { x /= s; y /= s; }

			T& operator[](int i)		{ return data[i]; }
	const	T& operator[](int i) const	{ return data[i]; }

	bool operator==(const Vec2& r) const { return x == r.x && y == r.y; }
	bool operator!=(const Vec2& r) const { return x != r.x || y != r.y; }
};

SGE_FORMATTER_T(class T, Vec2<T>)

using Vec2f = Vec2<float>;

}