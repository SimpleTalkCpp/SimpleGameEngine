#pragma once

#include "Vec4_Basic.h"

namespace sge {

template<class ELEMENT>
struct Mat4_Basic_Data {
	using Vec4 = Vec4_Basic<ELEMENT>;

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
	using Vec4 = typename DATA::Vec4;

	using DATA::cx;
	using DATA::cy;
	using DATA::cz;
	using DATA::cw;

	static const Mat4& identity();

	SGE_INLINE Mat4_Basic() = default;
	SGE_INLINE Mat4_Basic(const Vec4& cx_, const Vec4& cy_, const Vec4& cz_, const Vec4& cw_) 
		: DATA(cx_, cy_, cz_, cw_) {}

			Vec4& operator[](int i)			{ return _columns[i]; }
	const	Vec4& operator[](int i) const	{ return _columns[i]; }

	Vec4 col(int i) const	{ return _columns[i]; }
	Vec4 row(int i) const	{ return Vec4(cx[i], cy[i], cz[i], cw[i]); }

	void setCol(int i, const Vec4& v) { _columns[i] = v; }
	void setRow(int i, const Vec4& v) { cx[i] = v.x; cy[i] = v.y; cz[i] = v.z; cw[i] = v.w; }

	SGE_INLINE Mat4 operator*(const Mat4& r) const;

	SGE_INLINE Mat4 operator+(const Scalar& s) const { return Mat4(cx + s, cy + s, cz + s, cw + s); }
	SGE_INLINE Mat4 operator-(const Scalar& s) const { return Mat4(cx - s, cy - s, cz - s, cw - s); }

	SGE_INLINE Mat4 operator*(const Scalar& s) const { return Mat4(cx * s, cy * s, cz * s, cw * s); }
	SGE_INLINE Mat4 operator/(const Scalar& s) const { return Mat4(cx / s, cy / s, cz / s, cw / s); }

	SGE_INLINE void operator+=(const Scalar& s) { cx += s; cy += s; cz += s; cw += s; }
	SGE_INLINE void operator-=(const Scalar& s) { cx -= s; cy -= s; cz -= s; cw -= s; }
	SGE_INLINE void operator*=(const Scalar& s) { cx *= s; cy *= s; cz *= s; cw *= s; }
	SGE_INLINE void operator/=(const Scalar& s) { cx /= s; cy /= s; cz /= s; cw /= s; }

	SGE_INLINE bool operator==(const Mat4& r) const { return cx == r.cx && cy == r.cy && cz == r.cz && cw == r.cw; }
	SGE_INLINE bool operator!=(const Mat4& r) const { return cx != r.cx || cy != r.cy || cz != r.cz || cw != r.cw; }

	void onFormat(fmt::format_context& ctx) const {
		fmt::format_to(ctx.out(), "Mat4(\n  {},\n  {},\n  {},\n  {})", cx, cy, cz, cw);
	}
};

using Mat4f_Basic = Mat4_Basic<float >;
using Mat4d_Basic = Mat4_Basic<double>;

SGE_FORMATTER_T( SGE_ARGS(class T, class DATA), Mat4_Basic< SGE_ARGS(T, DATA) >)

template<class T, class DATA> SGE_INLINE
const Mat4_Basic<T, DATA> & Mat4_Basic<T, DATA>::identity() {
	static Mat4 s(	{1, 0, 0, 0},
					{0, 1, 0, 0},
					{0, 0, 1, 0},
					{0, 0, 0, 1});
	return s;
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


}
