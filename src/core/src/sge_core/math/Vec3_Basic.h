#pragma once

#include "Tuple3.h"
#include "Vec2.h"
#include "Rect2.h"

namespace sge {

template<class T> using Vec3_Basic_Data = Tuple3<T>;

template<class T, class DATA = Vec3_Basic_Data<T> >
struct Vec3_Basic : public DATA {
public:
	using Vec3 = Vec3_Basic;
	using Vec2 = Vec2<T>;

	static const size_t kElementCount = 3;

	using ElementType = typename DATA::ElementType;
	using DATA::x; // require this on gcc/clang, otherwise the fullname `Base::x` is needed instead of `x`
	using DATA::y;
	using DATA::z;

	using DATA::data;

	SGE_INLINE static Vec3 s_zero()		{ return Vec3(0,0,0); }
	SGE_INLINE static Vec3 s_one()		{ return Vec3(1,1,1); }

	SGE_INLINE static Vec3 s_forward()	{ return Vec3( 0, 0, 1); }
	SGE_INLINE static Vec3 s_back()		{ return Vec3( 0, 0,-1); }
	SGE_INLINE static Vec3 s_up()		{ return Vec3( 0, 1, 0); }
	SGE_INLINE static Vec3 s_down()		{ return Vec3( 0,-1, 0); }
	SGE_INLINE static Vec3 s_right()	{ return Vec3( 1 ,0, 0); }
	SGE_INLINE static Vec3 s_left()		{ return Vec3(-1 ,0, 0); }

	SGE_INLINE static Vec3 s_inf()		{ auto f = Math::inf<T>(); return Vec3(f,f,f); }

	SGE_INLINE Vec3() = default;
	SGE_INLINE Vec3(const Tuple3<T> & v) { set(v); }
	SGE_INLINE Vec3(const T& x_, const T& y_, const T& z_) { set(x_, y_, z_); }
	SGE_INLINE Vec3(const Vec2& v, const T& z_) { set(v.x, v.y, z_); }

	SGE_INLINE void set(const Tuple3<T> & v) { DATA::set(v); }
	SGE_INLINE void set(const T& x_, const T& y_, const T& z_) { set(Tuple3<T>(x_, y_, z_)); }

	SGE_INLINE bool equals(const Vec3& r, const T& epsilon = Math::epsilon<T>()) const;
	SGE_INLINE bool equals0(              const T& epsilon = Math::epsilon<T>()) const;

	SGE_INLINE void setAll(const T& v) { set(v,v,v); }
	SGE_INLINE bool isAll (const T& v) { return equals(Vec3(v,v,v)); }

//----
	SGE_INLINE Vec3 operator+(const Vec3& r) const { return Vec3(x + r.x, y + r.y, z + r.z); }
	SGE_INLINE Vec3 operator-(const Vec3& r) const { return Vec3(x - r.x, y - r.y, z - r.z); }
	SGE_INLINE Vec3 operator*(const Vec3& r) const { return Vec3(x * r.x, y * r.y, z * r.z); }
	SGE_INLINE Vec3 operator/(const Vec3& r) const { return Vec3(x / r.x, y / r.y, z / r.z); }

	SGE_INLINE Vec3 operator+(const T& s) const { return Vec3(x + s, y + s, z + s); }
	SGE_INLINE Vec3 operator-(const T& s) const { return Vec3(x - s, y - s, z - s); }
	SGE_INLINE Vec3 operator*(const T& s) const { return Vec3(x * s, y * s, z * s); }
	SGE_INLINE Vec3 operator/(const T& s) const { return Vec3(x / s, y / s, z / s); }

	SGE_INLINE void operator+=(const Vec3& r) { x += r.x; y += r.y; z += r.z; }
	SGE_INLINE void operator-=(const Vec3& r) { x -= r.x; y -= r.y; z -= r.z; }
	SGE_INLINE void operator*=(const Vec3& r) { x *= r.x; y *= r.y; z *= r.z; }
	SGE_INLINE void operator/=(const Vec3& r) { x /= r.x; y /= r.y; z /= r.z; }

	SGE_INLINE void operator+=(const T& s) { x += s; y += s; z += s; }
	SGE_INLINE void operator-=(const T& s) { x -= s; y -= s; z -= s; }
	SGE_INLINE void operator*=(const T& s) { x *= s; y *= s; z *= s; }
	SGE_INLINE void operator/=(const T& s) { x /= s; y /= s; z /= s; }

	SGE_INLINE bool operator==(const Vec3& r) const { return x == r.x && y == r.y && z == r.z; }
	SGE_INLINE bool operator!=(const Vec3& r) const { return x != r.x || y != r.y || z != r.z; }

			T& operator[](int i)		{ return data[i]; }
	const	T& operator[](int i) const	{ return data[i]; }

	SGE_NODISCARD SGE_INLINE Vec3	cross	(const Vec3& v) const	{ return Vec3(y*v.z - z*v.y, z*v.x - x*v.z,x*v.y - y*v.x); }
	SGE_NODISCARD SGE_INLINE T		dot		(const Vec3& v) const	{ return (x*v.x) + (y*v.y) + (z*v.z); }

	SGE_NODISCARD SGE_INLINE T		magnitude		() const	{ return Math::sqrt (sqrMagnitude()); }
	SGE_NODISCARD SGE_INLINE T		sqrMagnitude	() const	{ return dot(*this); }

	SGE_NODISCARD SGE_INLINE T		length			() const	{ return magnitude(); }
	SGE_NODISCARD SGE_INLINE T		sqrLength		() const	{ return sqrMagnitude(); }

	SGE_NODISCARD SGE_INLINE T		distance		(const Vec3 &r) const	{ return (*this - r).length();    }
	SGE_NODISCARD SGE_INLINE T		sqrDistance		(const Vec3 &r) const	{ return (*this - r).sqrLength(); }

	SGE_NODISCARD Vec3 normalize() const { T m = magnitude(); return Math::equals0(m) ? s_zero() : (*this / m); }

	SGE_INLINE	Vec2	xy() const { return Vec2(x,y); }
	SGE_INLINE	Vec2	xz() const { return Vec2(x,z); }

	Tuple3<T> toTuple() const { return Tuple3<T>(x,y,z); }
	operator Tuple3<T>() const { return toTuple(); }

	void onFormat(fmt::format_context& ctx) const {
		fmt::format_to(ctx.out(), "({}, {}, {})", x, y, z);
	}

	template<class R>
	static Vec3 s_cast(const Vec3_Basic<R>& r) { return Vec3(static_cast<T>(r.x), static_cast<T>(r.y), static_cast<T>(r.z)); }
};

using Vec3f_Basic = Vec3_Basic<float>;
using Vec3d_Basic = Vec3_Basic<double>;

// work around for comma
// SGE_FORMATTER_T(class T SGE_COMMA class DATA, Vec3_Basic<T SGE_COMMA DATA>)

// another work around for comma
SGE_FORMATTER_T( SGE_ARGS(class T, class DATA), Vec3_Basic< SGE_ARGS(T, DATA) >)


template<class T, class DATA> SGE_INLINE
bool Vec3_Basic<T, DATA>::equals(const Vec3& r, const T& epsilon) const {
	return Math::equals(x, r.x, epsilon)
		&& Math::equals(y, r.y, epsilon)
		&& Math::equals(z, r.z, epsilon);
}

template<class T, class DATA> SGE_INLINE
bool Vec3_Basic<T, DATA>::equals0(const T& epsilon) const {
	return Math::equals0(x, epsilon)
		&& Math::equals0(y, epsilon)
		&& Math::equals0(z, epsilon);
}

namespace Math {

template<class T, class DATA> SGE_INLINE
void sincos(const Vec3_Basic<T, DATA>& th, Vec3_Basic<T, DATA>& outSin, Vec3_Basic<T, DATA>& outCos) {
	Math::sincos(th.x, outSin.x, outCos.x);
	Math::sincos(th.y, outSin.y, outCos.y);
	Math::sincos(th.z, outSin.z, outCos.z);
}

template<class T, class DATA> SGE_INLINE
Vec3_Basic<T, DATA> min(const Vec3_Basic<T, DATA>& a, const Vec3_Basic<T, DATA>& b) {
	return Vec3_Basic<T, DATA>(	Math::min(a.x, b.x),
								Math::min(a.y, b.y),
								Math::min(a.z, b.z));
}

template<class T, class DATA> SGE_INLINE
Vec3_Basic<T, DATA> max(const Vec3_Basic<T, DATA>& a, const Vec3_Basic<T, DATA>& b) {
	return Vec3_Basic<T, DATA>(	Math::max(a.x, b.x),
								Math::max(a.y, b.y),
								Math::max(a.z, b.z));
}

}

}
