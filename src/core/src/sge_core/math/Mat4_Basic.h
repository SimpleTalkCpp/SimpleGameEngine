#pragma once

#include "Vec4.h"
#include "Rect2.h"
#include "Quat4.h"

namespace sge {

template<class ELEMENT>
struct Mat4_Basic_Data {
	using Vec4 = Vec4<ELEMENT>;

	using ElementType = ELEMENT;
	static const size_t kElementCount = 16;

	union {
		struct{ Vec4 cx, cy, cz, cw; };
		Vec4 _columns[4];
		ELEMENT _elements[kElementCount];
	};

	SGE_INLINE Mat4_Basic_Data() = default;
	SGE_INLINE Mat4_Basic_Data(const Vec4& cx_, const Vec4& cy_, const Vec4& cz_, const Vec4& cw_) { set(cx_, cy_, cz_, cw_); }

	SGE_INLINE void set(const Vec4& cx_, const Vec4& cy_, const Vec4& cz_, const Vec4& cw_) {
		cx = cx_; cy = cy_; cz = cz_; cw = cw_;
	}

};

template<class T, class DATA = Mat4_Basic_Data<T> >
struct Mat4_Basic : public DATA {
	using Mat4 = Mat4_Basic;

	using ElementType = T;
	using Scalar = ElementType;
	using Vec4   = sge::Vec4<T>;
	using Vec3   = sge::Vec3<T>;
	using Rect2  = sge::Rect2<T>;
	using Quat4  = sge::Quat4<T>;

	using DATA::cx;
	using DATA::cy;
	using DATA::cz;
	using DATA::cw;

	static SGE_INLINE const Mat4&	s_identity();
	
	static SGE_INLINE		Mat4	s_translate	(const Vec3 & t);
	static SGE_INLINE		Mat4	s_rotate	(const Vec3 & r);
	static SGE_INLINE		Mat4	s_rotateX	(const T & rad);
	static SGE_INLINE		Mat4	s_rotateY	(const T & rad);
	static SGE_INLINE		Mat4	s_rotateZ	(const T & rad);
	static SGE_INLINE		Mat4	s_scale		(const Vec3 & s);
	static SGE_INLINE		Mat4	s_shear		(const Vec3 & v);

	static SGE_INLINE		Mat4	s_quat		(const Quat4& q);

	static SGE_INLINE		Mat4	s_TRS(const Vec3 & translate, const Vec3 & rotate, const Vec3 & scale);
	static SGE_INLINE		Mat4	s_TRS(const Vec3 & translate, const Quat4& rotate, const Vec3 & scale);
	static SGE_INLINE		Mat4	s_TS (const Vec3 & translate, const Vec3 & scale);

	static SGE_INLINE		Mat4	s_perspective	(T fovy_rad, T aspect, T zNear, T zFar);
	static SGE_INLINE		Mat4	s_ortho			(T left, T right, T bottom, T top, T zNear, T zFar);
	static SGE_INLINE		Mat4	s_lookAt		(const Vec3 & eye, const Vec3 & aim, const Vec3 & up);

	SGE_INLINE Mat4_Basic() = default;
	SGE_INLINE Mat4_Basic(const Vec4& cx_, const Vec4& cy_, const Vec4& cz_, const Vec4& cw_) 
		: DATA(cx_, cy_, cz_, cw_) {}

	SGE_INLINE			Vec4& operator[](int i)			{ return _columns[i]; }
	SGE_INLINE const	Vec4& operator[](int i) const	{ return _columns[i]; }

	SGE_INLINE Vec4 col(int i) const	{ return _columns[i]; }
	SGE_INLINE Vec4 row(int i) const	{ return Vec4(cx[i], cy[i], cz[i], cw[i]); }

	SGE_INLINE void setCol(int i, const Vec4& v) { _columns[i] = v; }
	SGE_INLINE void setRow(int i, const Vec4& v) { cx[i] = v.x; cy[i] = v.y; cz[i] = v.z; cw[i] = v.w; }

	SGE_INLINE Mat4 transpose() const;

	T determinant3x3() const;

	Mat4 inverse			() const;
	Mat4 inverse3x3			() const;
	Mat4 inverse3x3Transpose() const;

	SGE_INLINE Mat4 operator*(const Mat4& r) const;

	SGE_INLINE Mat4 operator+(const Scalar& s) const { Vec4 v(s,s,s,s); return Mat4(cx + v, cy + v, cz + v, cw + v); }
	SGE_INLINE Mat4 operator-(const Scalar& s) const { Vec4 v(s,s,s,s); return Mat4(cx - v, cy - v, cz - v, cw - v); }

	SGE_INLINE Mat4 operator*(const Scalar& s) const { Vec4 v(s,s,s,s); return Mat4(cx * v, cy * v, cz * v, cw * v); }
	SGE_INLINE Mat4 operator/(const Scalar& s) const { Vec4 v(s,s,s,s); return Mat4(cx / v, cy / v, cz / v, cw / v); }

	SGE_INLINE void operator+=(const Scalar& s) { Vec4 v(s,s,s,s); cx += v; cy += v; cz += v; cw += v; }
	SGE_INLINE void operator-=(const Scalar& s) { Vec4 v(s,s,s,s); cx -= v; cy -= v; cz -= v; cw -= v; }
	SGE_INLINE void operator*=(const Scalar& s) { Vec4 v(s,s,s,s); cx *= v; cy *= v; cz *= v; cw *= v; }
	SGE_INLINE void operator/=(const Scalar& s) { Vec4 v(s,s,s,s); cx /= v; cy /= v; cz /= v; cw /= v; }

	SGE_INLINE bool operator==(const Mat4& r) const { return cx == r.cx && cy == r.cy && cz == r.cz && cw == r.cw; }
	SGE_INLINE bool operator!=(const Mat4& r) const { return cx != r.cx || cy != r.cy || cz != r.cz || cw != r.cw; }

	SGE_INLINE Vec4	mulPoint	(const Vec4& v) const { return Vec4(cx * v.x + cy * v.y  + cz * v.z + cw * v.w); }

	// faster than mulPoint but no projection
	SGE_INLINE Vec3 mulPoint4x3	(const Vec3& v) const { return mulPoint(Vec4(v, 1)).xyz(); }

	// for vector (direction)
	SGE_INLINE Vec3 mulVector	(const Vec3& v) const { return Vec3(cx.xyz() * v.x + cy.xyz() * v.y + cz.xyz() * v.z); }

	// for normal non-uniform scale
	SGE_INLINE Vec3 mulNormal	(const Vec3& v) const {	return inverse3x3Transpose().mulVector(v); }

	Vec3 unprojectPoint(const Vec3& screenPos, const Rect2& viewport) const {
		return inverse().unprojectPointFromInv(screenPos, viewport); 
	}

	Vec3 unprojectPointFromInv(const Vec3& screenPos, const Rect2& viewport) const;

	void onFormat(fmt::format_context& ctx) const {
		fmt::format_to(ctx.out(), "Mat4(\n  {},\n  {},\n  {},\n  {})", cx, cy, cz, cw);
	}
};

using Mat4f_Basic = Mat4_Basic<float >;
using Mat4d_Basic = Mat4_Basic<double>;

SGE_FORMATTER_T( SGE_ARGS(class T, class DATA), Mat4_Basic< SGE_ARGS(T, DATA) >)

#if 0
#pragma mark ------------------- static functions -------------------
#endif

template<class T, class DATA> SGE_INLINE
const Mat4_Basic<T, DATA> & Mat4_Basic<T, DATA>::s_identity() {
	static Mat4 s(	{1, 0, 0, 0},
					{0, 1, 0, 0},
					{0, 0, 1, 0},
					{0, 0, 0, 1});
	return s;
}


template<class T, class DATA> SGE_INLINE
Mat4_Basic<T, DATA> Mat4_Basic<T, DATA>::s_translate(const Vec3& t) {
	return Mat4( {1,   0,   0,   0},
				 {0,   1,   0,   0},
				 {0,   0,   1,   0},
				 {t.x, t.y, t.z, 1});
}

template<class T, class DATA> SGE_INLINE
Mat4_Basic<T, DATA> Mat4_Basic<T, DATA>::s_rotate(const Vec3& rad) {
	if (rad.equals0()) return s_identity();

	Vec3 s, c;
	Math::sincos(rad.x, s.x, c.x);
	Math::sincos(rad.y, s.y, c.y);
	Math::sincos(rad.z, s.z, c.z);

	return Mat4(
		{(c.y*c.z), (s.x*s.y*c.z - c.x*s.z), (s.x*s.z + c.x*s.y*c.z), 0},
		{(c.y*s.z), (c.x*c.z + s.x*s.y*s.z), (c.x*s.y*s.z - s.x*c.z), 0},
		{(-s.y),    (s.x*c.y),               (c.x*c.y),               0},
		{0,         0,                        0,                      1}
	);
}

template<class T, class DATA> SGE_INLINE
Mat4_Basic<T, DATA> Mat4_Basic<T, DATA>::s_quat(const Quat4& q) {
	T qxx(q.x * q.x);
	T qyy(q.y * q.y);
	T qzz(q.z * q.z);
	T qxz(q.x * q.z);
	T qxy(q.x * q.y);
	T qyz(q.y * q.z);
	T qwx(q.w * q.x);
	T qwy(q.w * q.y);
	T qwz(q.w * q.z);

	return Mat4({T(1)-T(2)*(qyy+qzz), T(2)*(qxy+qwz),      T(2)*(qxz-qwy),      T(0)},
				{T(2)*(qxy-qwz),      T(1)-T(2)*(qxx+qzz), T(2)*(qyz+qwx),      T(0)},
				{T(2)*(qxz+qwy),      T(2)*(qyz-qwx),      T(1)-T(2)*(qxx+qyy), T(0)},
				{T(0),                T(0),                T(0),                T(1)});
}


template<class T, class DATA> SGE_INLINE
Mat4_Basic<T, DATA> Mat4_Basic<T, DATA>::s_rotateX(const T& rad) {
	if (Math::equals0(rad)) return s_identity();

	T s, c;
	Math::sincos(rad, s, c);
	return Mat4({1, 0, 0, 0},
				{0, c, s, 0},
				{0,-s, c, 0},
				{0, 0, 0, 1});
}

template<class T, class DATA> SGE_INLINE
Mat4_Basic<T, DATA> Mat4_Basic<T, DATA>::s_rotateY(const T& rad) {
	if (Math::equals0(rad)) return s_identity();

	T s, c;
	Math::sincos(rad, s, c);
	return Mat4({c, 0,-s, 0},
				{0, 1, 0, 0},
				{s, 0, c, 0},
				{0, 0, 0, 1});
}

template<class T, class DATA> SGE_INLINE
Mat4_Basic<T, DATA> Mat4_Basic<T, DATA>::s_rotateZ(const T& rad) {
	if (Math::equals0(rad)) return s_identity();

	T s, c;
	Math::sincos(rad, s, c);
	return Mat4({ c, s, 0, 0},
				{-s, c, 0, 0},
				{ 0, 0, 1, 0},
				{ 0, 0, 0, 1});
}

template<class T, class DATA> SGE_INLINE
Mat4_Basic<T, DATA> Mat4_Basic<T, DATA>::s_scale(const Vec3& s) {
	return Mat4({s.x, 0,   0,   0},
				{0,   s.y, 0,   0},
				{0,   0,   s.z, 0},
				{0,   0,   0,   1});
}


template<class T, class DATA> SGE_INLINE
Mat4_Basic<T, DATA> Mat4_Basic<T, DATA>::s_shear(const Vec3& v) {
	return Mat4( {  1,   0,  0,  0},
				 {v.x,   1,  0,  0},
				 {v.y, v.z,  1,  0},
				 {  0,   0,  0,  1});
}

template<class T, class DATA> SGE_INLINE 
Mat4_Basic<T, DATA> Mat4_Basic<T, DATA>::s_TRS(const Vec3& translate, const Vec3& rotate, const Vec3& scale) {
	Vec3 s, c;
	Math::sincos(rotate.x, s.x, c.x);
	Math::sincos(rotate.y, s.y, c.y);
	Math::sincos(rotate.z, s.z, c.z);

	return Mat4(
		{scale.x * (c.y*c.z),				scale.x * (c.y*s.z),				scale.x * (-s.y),		0},
		{scale.y * (s.x*s.y*c.z - c.x*s.z),	scale.y * (c.x*c.z + s.x*s.y*s.z),	scale.y * (s.x*c.y),	0},
		{scale.z * (s.x*s.z + c.x*s.y*c.z),	scale.z * (c.x*s.y*s.z - s.x*c.z),	scale.z * (c.x*c.y),	0},
		{translate.x,						translate.y,						translate.z,			1});
}


template<class T, class DATA> SGE_INLINE
Mat4_Basic<T, DATA> Mat4_Basic<T, DATA>::s_TRS(const Vec3& translate, const Quat4& rotate, const Vec3& scale) {
	return s_translate(translate) * s_quat(rotate) * s_scale(scale);
}

template<class T, class DATA> SGE_INLINE 
Mat4_Basic<T, DATA> Mat4_Basic<T, DATA>::s_TS(const Vec3& translate, const Vec3& scale) {
	return Mat4({scale.x, 0, 0, 0},
				{0, scale.y, 0, 0},
				{0, 0, scale.z, 0},
				{translate.x, translate.y, translate.z, 1});
}


template<class T, class DATA> SGE_INLINE
Mat4_Basic<T, DATA> Mat4_Basic<T, DATA>::s_perspective(T fovy_rad, T aspect, T zNear, T zFar) {
	if (Math::equals0(aspect)) {
		return s_identity();
	}

	T deltaZ = zFar - zNear;
	T tf = Math::tan(fovy_rad / T(2));
		
	return Mat4(
		{1 / (aspect * tf), 0,      0,                           0},
		{0,                 1 / tf, 0,                           0},
		{0,                 0,      -(zFar + zNear) / deltaZ,   -1},
		{0,                 0,      -2 * zNear * zFar / deltaZ,  0}
	);
}

template<class T, class DATA> SGE_INLINE
Mat4_Basic<T, DATA> Mat4_Basic<T, DATA>::s_ortho(T left, T right, T bottom, T top, T zNear, T zFar) {
	T w = right - left;
	T h = top - bottom;
	T d = zFar - zNear;

	if( w == 0 || h == 0 || d == 0 ) {
		return s_identity();
	}

	return Mat4(
		{2/w, 0,    0,   0},
		{0,   2/h,  0,   0},
		{0,   0,   -2/d, 0},
		{-(right+left) / w, -(top+bottom) / h, -(zFar+zNear) / d, 1}
	);
}

template<class T, class DATA> SGE_INLINE
Mat4_Basic<T, DATA> Mat4_Basic<T, DATA>::s_lookAt(const Vec3& eye, const Vec3& aim, const Vec3& up) {
	auto f = (aim - eye).normalize();
	auto s = f.cross(up).normalize();
	auto u = s.cross(f);

	return Mat4(
		{s.x,  u.x, -f.x, 0},
		{s.y,  u.y, -f.y, 0},
		{s.z,  u.z, -f.z, 0},
		{-s.dot(eye), -u.dot(eye), f.dot(eye), 1}
	);
}

#if 0
#pragma mark ------------------- instance functions -------------------
#endif

template<class T, class DATA>
T sge::Mat4_Basic<T, DATA>::determinant3x3() const {
	return cx.x * (cy.y * cz.z - cz.y * cy.z)
		 - cy.x * (cx.y * cz.z - cz.y * cx.z)
		 + cz.x * (cx.y * cy.z - cy.y * cx.z);
}

template<class T, class DATA>
Mat4_Basic<T, DATA> Mat4_Basic<T, DATA>::inverse3x3() const {
	T oneOverDeterminant = T(1) / determinant3x3();

	return Mat4(
		{
			(cy.y * cz.z - cz.y * cy.z) *  oneOverDeterminant, // xx
			(cx.y * cz.z - cz.y * cx.z) * -oneOverDeterminant, // xy
			(cx.y * cy.z - cy.y * cx.z) *  oneOverDeterminant, // xz
			0
		},
		{
			(cy.x * cz.z - cz.x * cy.z) * -oneOverDeterminant, // yx
			(cx.x * cz.z - cz.x * cx.z) *  oneOverDeterminant, // yy
			(cx.x * cy.z - cy.x * cx.z) * -oneOverDeterminant, // yz
			0
		},
		{
			(cy.x * cz.y - cz.x * cy.y) *  oneOverDeterminant, // zx
			(cx.x * cz.y - cz.x * cx.y) * -oneOverDeterminant, // zy
			(cx.x * cy.y - cy.x * cx.y) *  oneOverDeterminant, // zz
			0
		},
		{0,0,0,1}
	);
}

template<class T, class DATA>
Mat4_Basic<T, DATA> Mat4_Basic<T, DATA>::inverse3x3Transpose() const {
	T oneOverDeterminant = T(1) / determinant3x3();

	return Mat4(
		{
			(cy.y * cz.z - cz.y * cy.z) *  oneOverDeterminant, // xx
			(cy.x * cz.z - cz.x * cy.z) * -oneOverDeterminant, // yx
			(cy.x * cz.y - cz.x * cy.y) *  oneOverDeterminant, // zx
			0
		},
		{
			(cx.y * cz.z - cz.y * cx.z) * -oneOverDeterminant, // xy
			(cx.x * cz.z - cz.x * cx.z) *  oneOverDeterminant, // yy
			(cx.x * cz.y - cz.x * cx.y) * -oneOverDeterminant, // zy
			0
		},
		{
			(cx.y * cy.z - cy.y * cx.z) *  oneOverDeterminant, // xz
			(cx.x * cy.z - cy.x * cx.z) * -oneOverDeterminant, // yz
			(cx.x * cy.y - cy.x * cx.y) *  oneOverDeterminant, // zz
			0
		},
		{0,0,0,1}
	);
}

template<class T, class DATA> SGE_INLINE
Mat4_Basic<T, DATA> Mat4_Basic<T, DATA>::inverse() const {
	T coef00 = cz.z * cw.w - cw.z * cz.w;
	T coef02 = cy.z * cw.w - cw.z * cy.w;
	T coef03 = cy.z * cz.w - cz.z * cy.w;

	T coef04 = cz.y * cw.w - cw.y * cz.w;
	T coef06 = cy.y * cw.w - cw.y * cy.w;
	T coef07 = cy.y * cz.w - cz.y * cy.w;

	T coef08 = cz.y * cw.z - cw.y * cz.z;
	T coef10 = cy.y * cw.z - cw.y * cy.z;
	T coef11 = cy.y * cz.z - cz.y * cy.z;

	T coef12 = cz.x * cw.w - cw.x * cz.w;
	T coef14 = cy.x * cw.w - cw.x * cy.w;
	T coef15 = cy.x * cz.w - cz.x * cy.w;

	T coef16 = cz.x * cw.z - cw.x * cz.z;
	T coef18 = cy.x * cw.z - cw.x * cy.z;
	T coef19 = cy.x * cz.z - cz.x * cy.z;

	T coef20 = cz.x * cw.y - cw.x * cz.y;
	T coef22 = cy.x * cw.y - cw.x * cy.y;
	T coef23 = cy.x * cz.y - cz.x * cy.y;

	Vec4 fac0(coef00, coef00, coef02, coef03);
	Vec4 fac1(coef04, coef04, coef06, coef07);
	Vec4 fac2(coef08, coef08, coef10, coef11);
	Vec4 fac3(coef12, coef12, coef14, coef15);
	Vec4 fac4(coef16, coef16, coef18, coef19);
	Vec4 fac5(coef20, coef20, coef22, coef23);

	Vec4 v0(cy.x, cx.x, cx.x, cx.x);
	Vec4 v1(cy.y, cx.y, cx.y, cx.y);
	Vec4 v2(cy.z, cx.z, cx.z, cx.z);
	Vec4 v3(cy.w, cx.w, cx.w, cx.w);

	Vec4 signA(+1, -1, +1, -1);
	Vec4 signB(-1, +1, -1, +1);

	Vec4 inv0(v1 * fac0 - v2 * fac1 + v3 * fac2);
	Vec4 inv1(v0 * fac0 - v2 * fac3 + v3 * fac4);
	Vec4 inv2(v0 * fac1 - v1 * fac3 + v3 * fac5);
	Vec4 inv3(v0 * fac2 - v1 * fac4 + v2 * fac5);

	Mat4 inv(inv0 * signA,
			 inv1 * signB,
			 inv2 * signA,
			 inv3 * signB);

	Vec4 dot0(cx * inv.row(0));
	T dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);
	T oneOverDeterminant = T(1) / dot1;

	return inv * oneOverDeterminant;
}

template<class T, class DATA> SGE_INLINE
Mat4_Basic<T, DATA> Mat4_Basic<T, DATA>::transpose() const {
	return Mat4({ cx.x, cy.x, cz.x, cw.x },
				{ cx.y, cy.y, cz.y, cw.y },
				{ cx.z, cy.z, cz.z, cw.z },
				{ cx.w, cy.w, cz.w, cw.w });
}

template<class T, class DATA> SGE_INLINE
Mat4_Basic<T, DATA> sge::Mat4_Basic<T, DATA>::operator*(const Mat4& r) const {
	Mat4 o;
	T e0, e1, e2, e3;

	e0 = cx.x, e1 = cy.x, e2 = cz.x, e3 = cw.x;
	o.cx.x = e0*r.cx.x + e1*r.cx.y + e2*r.cx.z + e3*r.cx.w;
	o.cy.x = e0*r.cy.x + e1*r.cy.y + e2*r.cy.z + e3*r.cy.w;
	o.cz.x = e0*r.cz.x + e1*r.cz.y + e2*r.cz.z + e3*r.cz.w;
	o.cw.x = e0*r.cw.x + e1*r.cw.y + e2*r.cw.z + e3*r.cw.w;

	e0 = cx.y, e1 = cy.y, e2 = cz.y, e3 = cw.y;
	o.cx.y = e0*r.cx.x + e1*r.cx.y + e2*r.cx.z + e3*r.cx.w;
	o.cy.y = e0*r.cy.x + e1*r.cy.y + e2*r.cy.z + e3*r.cy.w;
	o.cz.y = e0*r.cz.x + e1*r.cz.y + e2*r.cz.z + e3*r.cz.w;
	o.cw.y = e0*r.cw.x + e1*r.cw.y + e2*r.cw.z + e3*r.cw.w;

	e0 = cx.z, e1 = cy.z, e2 = cz.z, e3 = cw.z;
	o.cx.z = e0*r.cx.x + e1*r.cx.y + e2*r.cx.z + e3*r.cx.w;
	o.cy.z = e0*r.cy.x + e1*r.cy.y + e2*r.cy.z + e3*r.cy.w;
	o.cz.z = e0*r.cz.x + e1*r.cz.y + e2*r.cz.z + e3*r.cz.w;
	o.cw.z = e0*r.cw.x + e1*r.cw.y + e2*r.cw.z + e3*r.cw.w;

	e0 = cx.w, e1 = cy.w, e2 = cz.w, e3 = cw.w;
	o.cx.w = e0*r.cx.x + e1*r.cx.y + e2*r.cx.z + e3*r.cx.w;
	o.cy.w = e0*r.cy.x + e1*r.cy.y + e2*r.cy.z + e3*r.cy.w;
	o.cz.w = e0*r.cz.x + e1*r.cz.y + e2*r.cz.z + e3*r.cz.w;
	o.cw.w = e0*r.cw.x + e1*r.cw.y + e2*r.cw.z + e3*r.cw.w;

	return o;
}

template<class T, class DATA> SGE_INLINE
Vec3<T> sge::Mat4_Basic<T, DATA>::unprojectPointFromInv(const Vec3& screenPos, const Rect2& viewport) const {
	auto  tmp = Vec4(screenPos, 1);
	tmp.y = viewport.h - tmp.y; // y is down

	tmp.x = (tmp.x - viewport.x) / viewport.w * 2 - 1;
	tmp.y = (tmp.y - viewport.y) / viewport.h * 2 - 1;

	auto obj = mulPoint(tmp);
	return obj.toVec3();
}

}
