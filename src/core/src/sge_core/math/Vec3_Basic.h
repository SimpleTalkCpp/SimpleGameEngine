#pragma once

#include "Tuple3.h"
#include "Vec3.h"

namespace sge {

template<class T, class DATA = Tuple3<T> >
struct Vec3_Basic : public DATA {
public:
	using ElementType = T;
	static const size_t kElementCount = 3;

	using Vec3 = Vec3_Basic;

	using DATA::x; // require this on gcc/clang, otherwise the fullname `Base::x` is needed instead of `x`
	using DATA::y;
	using DATA::z;
	using DATA::data;

	SGE_INLINE Vec3() = default;
	SGE_INLINE Vec3(const T& x_, const T& y_, const T& z_) : DATA(x_, y_, z_) {}
	SGE_INLINE Vec3(const Tuple3<T> & v) : DATA(v) {}

	SGE_INLINE Vec3 operator+(const Vec3& r) const { return Vec3(x + r.x, y + r.y, z + r.z); }
	SGE_INLINE Vec3 operator-(const Vec3& r) const { return Vec3(x - r.x, y - r.y, z - r.z); }
	SGE_INLINE Vec3 operator*(const Vec3& r) const { return Vec3(x * r.x, y * r.y, z * r.z); }
	SGE_INLINE Vec3 operator/(const Vec3& r) const { return Vec3(x / r.x, y / r.y, z / r.z); }

	SGE_INLINE Vec3 operator+(const T& s) const { return Vec3(x + s, y + s, z + s); }
	SGE_INLINE Vec3 operator-(const T& s) const { return Vec3(x - s, y - s, z - s); }
	SGE_INLINE Vec3 operator*(const T& s) const { return Vec3(x * s, y * s, z * s); }
	SGE_INLINE Vec3 operator/(const T& s) const { return Vec3(x / s, y / s, z / s); }

	SGE_INLINE bool operator==(const Vec3& r) const { return x == r.x && y == r.y && z == r.z; }
	SGE_INLINE bool operator!=(const Vec3& r) const { return x != r.x || y != r.y || z != r.z; }

	void onFormat(fmt::format_context& ctx) const {
		fmt::format_to(ctx.out(), "({}, {}, {})", x, y, z);
	}
};

using Vec3f_Basic = Vec3_Basic<float>;
using Vec3d_Basic = Vec3_Basic<double>;

}

template<class T, class OPT>
struct fmt::formatter< sge::Vec3_Basic<T, OPT> > {
	using V = sge::Vec3_Basic<T, OPT>;

	auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }
	auto format(const V& v, fmt::format_context& ctx) {
		v.onFormat(ctx);
		return ctx.out();
	}
};
