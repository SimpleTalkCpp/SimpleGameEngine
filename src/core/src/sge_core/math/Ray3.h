#pragma once

#include "Mat4.h"
#include "MathGeometry.h"

namespace sge {

template<class T>
struct Ray3 {
private:
	using Vec2		= sge::Vec2<T>;
	using Vec3		= sge::Vec3<T>;
	using Vec4		= sge::Vec4<T>;
	using Mat4		= sge::Mat4<T>;
	using Rect2		= sge::Rect2<T>;
	using Line3		= sge::Math::Line3<T>;
public:

	Vec3 origin, dir;

	Ray3() = default;
	Ray3(const Vec3& origin_, const Vec3& dir_) : origin(origin_), dir(dir_) {}

	constexpr static Ray3 s_zero() { return Ray3(Vec3::s_zero(), Vec3::s_zero()); }

	static Ray3 s_unprojectFromInvMatrix(const Vec2& screenPos, const Mat4& invMat, const Rect2& viewport);

	struct HitTestResult {
		T		distance = Math::inf<T>();
		Vec3	point;
		Vec3	normal;

		bool	hasResult() const { return distance != Math::inf<T>(); }
	};

	bool getClosestPoint(Vec3& outPoint, const Vec3& inPoint) const;
	bool getClosestPoint(Vec3& outPoint, const Line3& line, T minOnLine = 0, T maxOnLine = 1) const;
};

using Ray3f = Ray3<float>;
using Ray3d = Ray3<double>;

}
