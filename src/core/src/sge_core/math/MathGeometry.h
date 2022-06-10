#pragma once

#include "Quat4.h"

namespace sge {
namespace Math {

template<class T>
struct Sphere3 {
	using Vec3 = sge::Vec3<T>;
public:
	Sphere3() = default;
	Sphere3(const Vec3& center_, T radius_) : center(center_), radius(radius_) {}
	void onFormat(fmt::format_context& f) const;

	Vec3	center;
	T		radius;
};

template<class T>
struct Triangle3 {
	using Vec3 = sge::Vec3<T>;
public:
	Triangle3() = default;
	Triangle3(const Vec3& v0_, const Vec3& v1_, const Vec3& v2_) : v0(v0_), v1(v1_), v2(v2_) {}

	Vec3 v0, v1, v2;
};

template<class T>
struct Plane3 {
	using Vec3 = sge::Vec3<T>;
public:
	Plane3() = default;
	Plane3(const Vec3& normal_, T distance_)
		: normal(normal_)
		, distance(distance_) 
	{}

	Plane3(const Vec3& normal_, const Vec3& point_)
		: normal(normal_)
		, distance(normal_.dot(point_))
	{}

	Plane3(const Triangle3<T>&  tri) {
		normal = (tri.v1 - tri.v0).cross(tri.v2 - tri.v0).normal();
		distance = normal.dot(tri.v0);
	}

	void onFormat(fmt::format_context& f) const;

	Vec3	normal;
	T		distance; // distance from origin
};

template<class T>
struct Cylinder3 {
	using Vec3 = sge::Vec3<T>;
public:
	Cylinder3() = default;
	Cylinder3(const Vec3& start_, const Vec3& end_, const T& radius_)
		: start(start_)
		, end(end_)
		, radius(radius_)
	{}

	Vec3	start;
	Vec3	end;
	T		radius;
};

template<class T>
struct Capsule3 {
	using Vec3 = sge::Vec3<T>;
public:
	Capsule3() = default;
	Capsule3(const Vec3& start_, const Vec3& end_, const T& radius_)
		: start(start_)
		, end(end_)
		, radius(radius_)
	{}

	Vec3	start;
	Vec3	end;
	T		radius;
};


template<class T>
struct Line2 {
	using Vec2 = sge::Vec2<T>;
public:
	Line2() = default;
	Line2(const Vec2& start_, const Vec2& end_)
		: start(start_)
		, end(end_)
	{}

	bool getClosestPoint(Vec2& outPoint, const Vec2& inPoint) const;
	T	distanceToPoint(const Vec2& pt) const;

	Vec2	start;
	Vec2	end;
};

template<class T>
struct Line3 {
	using Vec3 = sge::Vec3<T>;
public:
	Line3() = default;
	Line3(const Vec3& start_, const Vec3& end_)
		: start(start_)
		, end(end_)
	{}

	bool getClosestPoint(Vec3& outPoint, const Vec3& inPoint) const;

	T	distanceToPoint(const Vec3& pt) const;

	Vec3	start;
	Vec3	end;
};

//-----------------------------------------
using Sphere3f = Sphere3<float>;
using Sphere3d = Sphere3<double>;

using Plane3f  = Plane3<float>;
using Plane3d  = Plane3<double>;

using Triangle3f = Triangle3<float>;
using Triangle3d = Triangle3<double>;

using Capsule3f = Capsule3<float>;
using Capsule3d = Capsule3<double>;

using Line2f = Line2<float>;
using Line2d = Line2<double>;

using Line3f = Line3<float>;
using Line3d = Line3<double>;

} // namespace Math
} // namespace sge