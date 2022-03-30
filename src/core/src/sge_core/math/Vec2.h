#pragma once

namespace sge {

template<class T>
class Vec2 {
public:
	using ElementType = T;
	static const size_t kElementCount = 2;

	union {
		struct { T x, y; };
		T data[kElementCount];
	};

	Vec2() = default;
	Vec2(const T& x_, const T& y_) : x(x_), y(y_) {}

	Vec2 operator+(const Vec2& r) const { return Vec2(x + r.x, y + r.y); }
	Vec2 operator-(const Vec2& r) const { return Vec2(x - r.x, y - r.y); }
	Vec2 operator*(const Vec2& r) const { return Vec2(x * r.x, y * r.y); }
	Vec2 operator/(const Vec2& r) const { return Vec2(x / r.x, y / r.y); }

	Vec2 operator+(const T& s) const { return Vec2(x + s, y + s); }
	Vec2 operator-(const T& s) const { return Vec2(x - s, y - s); }
	Vec2 operator*(const T& s) const { return Vec2(x * s, y * s); }
	Vec2 operator/(const T& s) const { return Vec2(x / s, y / s); }
};

using Vec2f = Vec2<float>;

}