#include "Ray3.h"

namespace sge {

template<class T>
Ray3<T> Ray3<T>::s_unprojectFromInvMatrix(const Vec2& screenPos, const Mat4& invMat, const Rect2& viewport) {
	auto pt = screenPos;

	Vec3 p0(pt, 0);
	Vec3 p1(pt, 1);

	auto v0 = invMat.unprojectPointFromInv(p0, viewport);
	auto v1 = invMat.unprojectPointFromInv(p1, viewport);

	Ray3 o;
	o.origin = v0;
	o.dir = (v1 - v0).normalize();
	return o;
}

template<class T>
bool Ray3<T>::getClosestPoint(Vec3& outPoint, const Vec3& inPoint) const {
	auto v = inPoint - origin;
	auto t = v.dot(dir);
	outPoint = (t <= 0) ? origin : origin + dir * t;
	return true;
}

template<class T>
bool Ray3<T>::getClosestPoint(Vec3& outPoint, const Line3& line, T minOnLine, T maxOnLine) const {
	auto da = line.end - line.start;
	auto db = dir;
	auto dc = origin - line.start;

	auto ab = da.cross(db);
	if (ab.equals0())
		return false;

	auto s = dc.cross(db).dot( da.cross(db) ) / ab.sqrLength();
	s = Math::clamp<T>(s, minOnLine, maxOnLine);
	outPoint = line.start + da * s;
	return true;
}

// explicit specialization to force VisualC check syntax in function body
template struct Ray3<float>;
template struct Ray3<double>;

}