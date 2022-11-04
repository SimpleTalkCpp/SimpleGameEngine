#pragma once

#include "Tuple4.h"
#include "Vec3.h"

namespace sge {

template<class T> using Vec4_Basic_Data = Tuple4<T>;

template<class T, class DATA = Vec4_Basic_Data<T> >
struct Vec4_Basic : public DATA {
public:	
	using Vec4 = Vec4_Basic;
	static const size_t kElementCount = 4;

	using ElementType = typename DATA::ElementType;
	using DATA::x; // require this on gcc/clang, otherwise the fullname `Base::x` is needed instead of `x`
	using DATA::y;
	using DATA::z;
	using DATA::w;
	using DATA::data;

	using Vec2 = Vec2<T>;
	using Vec3 = Vec3<T>;

	SGE_INLINE Vec4() = default;
	SGE_INLINE Vec4(const Tuple4<T> & v) { set(v); }
	SGE_INLINE Vec4(const T& x_, const T& y_, const T& z_, const T& w_) { set(x_, y_, z_, w_); }
	SGE_INLINE Vec4(const Vec3& v, const T& w_) { set(v.x, v.y, v.z, w_); }

	SGE_INLINE void set(const Tuple4<T> & v) { DATA::set(v); }
	SGE_INLINE void set(const T& x_, const T& y_, const T& z_, const T& w_) { set(Tuple4<T>(x_, y_, z_, w_)); }

	SGE_INLINE bool equals(const Vec4& r, const T& epsilon = Math::epsilon<T>()) const;
	SGE_INLINE bool equals0(              const T& epsilon = Math::epsilon<T>()) const;

	SGE_INLINE void setAll(const T& v) { set(v,v,v,v); }
	SGE_INLINE bool isAll (const T& v) { return operator==(Vec4(v,v,v,v)); }

	SGE_INLINE Vec4 operator+(const Vec4& r) const { return Vec4(x + r.x, y + r.y, z + r.z, w + r.w); }
	SGE_INLINE Vec4 operator-(const Vec4& r) const { return Vec4(x - r.x, y - r.y, z - r.z, w - r.w); }
	SGE_INLINE Vec4 operator*(const Vec4& r) const { return Vec4(x * r.x, y * r.y, z * r.z, w * r.w); }
	SGE_INLINE Vec4 operator/(const Vec4& r) const { return Vec4(x / r.x, y / r.y, z / r.z, w / r.w); }

	// Scalar operators
	SGE_INLINE Vec4 operator+(const T& s) const { return Vec4(x + s, y + s, z + s, w + s); }
	SGE_INLINE Vec4 operator-(const T& s) const { return Vec4(x - s, y - s, z - s, w - s); }
	SGE_INLINE Vec4 operator*(const T& s) const { return Vec4(x * s, y * s, z * s, w * s); }
	SGE_INLINE Vec4 operator/(const T& s) const { return Vec4(x / s, y / s, z / s, w / s); }

	SGE_INLINE void operator+=(const Vec4& r) { x += r.x; y += r.y; z += r.z; w += r.w; }
	SGE_INLINE void operator-=(const Vec4& r) { x -= r.x; y -= r.y; z -= r.z; w -= r.w; }
	SGE_INLINE void operator*=(const Vec4& r) { x *= r.x; y *= r.y; z *= r.z; w *= r.w; }
	SGE_INLINE void operator/=(const Vec4& r) { x /= r.x; y /= r.y; z /= r.z; w /= r.w; }

	SGE_INLINE void operator+=(const T& s) { x += s; y += s; z += s; w += s; }
	SGE_INLINE void operator-=(const T& s) { x -= s; y -= s; z -= s; w -= s; }
	SGE_INLINE void operator*=(const T& s) { x *= s; y *= s; z *= s; w *= s; }
	SGE_INLINE void operator/=(const T& s) { x /= s; y /= s; z /= s; w /= s; }

	SGE_INLINE bool operator==(const Vec4& r) const { return x == r.x && y == r.y && z == r.z && w == r.w; }
	SGE_INLINE bool operator!=(const Vec4& r) const { return x != r.x || y != r.y || z != r.z || w != r.w; }

			T& operator[](int i)		{ return data[i]; }
	const	T& operator[](int i) const	{ return data[i]; }

	Vec2 xy		() const { return Vec2(x,y); }
	Vec2 xz		() const { return Vec2(x,z); }
	Vec2 yz		() const { return Vec2(y,z); }

	Vec3 xyz	() const { return Vec3(x,y,z); }
	Vec3 xy0	() const { return Vec3(x,y,0); }
	Vec3 x0z	() const { return Vec3(x,0,z); }

	Vec3 toVec3	() const { return (*this / w).xyz(); };

	Tuple4<T> toTuple() const { return Tuple4<T>(x,y,z,w); }
	operator Tuple4<T>() const { return toTuple(); }

	void onFormat(fmt::format_context& ctx) const {
		fmt::format_to(ctx.out(), "({}, {}, {}, {})", x, y, z, w);
	}

	template<class R>
	static Vec4 s_cast(const Vec4_Basic<R>& r) { return Vec4(static_cast<T>(r.x), static_cast<T>(r.y), static_cast<T>(r.z), static_cast<T>(r.w)); }
};

using Vec4f_Basic = Vec4_Basic<float>;
using Vec4d_Basic = Vec4_Basic<double>;

SGE_FORMATTER_T( SGE_ARGS(class T, class DATA), Vec4_Basic< SGE_ARGS(T, DATA) >)


template<class T, class DATA> SGE_INLINE
bool Vec4_Basic<T, DATA>::equals(const Vec4& r, const T& epsilon) const {
	return Math::equals(x, r.x, epsilon)
		&& Math::equals(y, r.y, epsilon)
		&& Math::equals(z, r.z, epsilon)
		&& Math::equals(w, r.w, epsilon);
}

template<class T, class DATA> SGE_INLINE
bool Vec4_Basic<T, DATA>::equals0(const T& epsilon) const {
	return Math::equals0(x, epsilon)
		&& Math::equals0(y, epsilon)
		&& Math::equals0(z, epsilon)
		&& Math::equals0(w, epsilon);
}

namespace Math {

template<class T, class DATA> SGE_INLINE
void sincos(const Vec4_Basic<T, DATA>& th, Vec4_Basic<T, DATA>& outSin, Vec4_Basic<T, DATA>& outCos) {
	Math::sincos(th.x, outSin.x, outCos.x);
	Math::sincos(th.y, outSin.y, outCos.y);
	Math::sincos(th.z, outSin.z, outCos.z);
	Math::sincos(th.w, outSin.w, outCos.w);
}

template<class T, class DATA> SGE_INLINE
Vec4_Basic<T, DATA> min(const Vec4_Basic<T, DATA>& a, const Vec4_Basic<T, DATA>& b) {
	return Vec4_Basic<T, DATA>(	Math::min(a.x, b.x),
								Math::min(a.y, b.y),
								Math::min(a.z, b.z),
								Math::min(a.w, b.w));
}

template<class T, class DATA> SGE_INLINE
Vec4_Basic<T, DATA> max(const Vec4_Basic<T, DATA>& a, const Vec4_Basic<T, DATA>& b) {
	return Vec4_Basic<T, DATA>(	Math::max(a.x, b.x),
								Math::max(a.y, b.y),
								Math::max(a.z, b.z),
								Math::max(a.w, b.w));
}

}

}
