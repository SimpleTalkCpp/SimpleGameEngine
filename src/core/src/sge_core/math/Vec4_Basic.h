#pragma once

#include "Tuple4.h"
#include "Vec3.h"

namespace sge {

template<class T, class DATA = Tuple4<T> >
struct Vec4_Basic : public DATA {
public:
	using ElementType = T;
	static const size_t kElementCount = 4;

	using Vec4 = Vec4_Basic;

	using DATA::x; // require this on gcc/clang, otherwise the fullname `Base::x` is needed instead of `x`
	using DATA::y;
	using DATA::z;
	using DATA::data;

	SGE_INLINE Vec4() = default;
	SGE_INLINE Vec4(const T& x_, const T& y_, const T& z_, const T& w_) : DATA(x_, y_, z_, w_) {}
	SGE_INLINE Vec4(const Tuple4<T> & v) : DATA(v) {}

	SGE_INLINE Vec4 operator+(const Vec4& r) const { return Vec4(x + r.x, y + r.y, z + r.z, w + r.w); }
	SGE_INLINE Vec4 operator-(const Vec4& r) const { return Vec4(x - r.x, y - r.y, z - r.z, w - r.w); }
	SGE_INLINE Vec4 operator*(const Vec4& r) const { return Vec4(x * r.x, y * r.y, z * r.z, w * r.w); }
	SGE_INLINE Vec4 operator/(const Vec4& r) const { return Vec4(x / r.x, y / r.y, z / r.z, w / r.w); }

	SGE_INLINE Vec4 operator+(const T& s) const { return Vec4(x + s, y + s, z + s, w + s); }
	SGE_INLINE Vec4 operator-(const T& s) const { return Vec4(x - s, y - s, z - s, w - s); }
	SGE_INLINE Vec4 operator*(const T& s) const { return Vec4(x * s, y * s, z * s, w * s); }
	SGE_INLINE Vec4 operator/(const T& s) const { return Vec4(x / s, y / s, z / s, w / s); }

	SGE_INLINE bool operator==(const Vec4& r) const { return x == r.x && y == r.y && z == r.z && w == r.w; }
	SGE_INLINE bool operator!=(const Vec4& r) const { return x != r.x || y != r.y || z != r.z || w != r.w; }

	void onFormat(fmt::format_context& ctx) const {
		fmt::format_to(ctx.out(), "({}, {}, {}, {})", x, y, z, w);
	}
};

using Vec4f_Basic = Vec4_Basic<float>;
using Vec4d_Basic = Vec4_Basic<double>;

}

template<class T, class OPT>
struct fmt::formatter< sge::Vec4_Basic<T, OPT> > {
	using V = sge::Vec4_Basic<T, OPT>;

	auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }
	auto format(const V& v, fmt::format_context& ctx) {
		v.onFormat(ctx);
		return ctx.out();
	}
};
