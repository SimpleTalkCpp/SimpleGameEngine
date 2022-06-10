#pragma once

template<class T>
struct MathxSphere3 {
	using Vec3 = sge::Vec3<T>;
public:
	MathxSphere3() = default;
	MathxSphere3(const Vec3& center_, T radius_) : center(center_), radius(radius_) {}
	void onFormat(fmt::format_context& f) const;

	Vec3	center;
	T		radius;
};

template<class T>
struct MathTriangle3 {
	using Vec3 = sge::Vec3<T>;
public:
	MathTriangle3() = default;
	MathTriangle3(const Vec3& v0_, const Vec3& v1_, const Vec3& v2_) : v0(v0_), v1(v1_), v2(v2_) {}

	Vec3 v0, v1, v2;
};

template<class T>
struct MathPlane3 {
	using Vec3 = sge::Vec3<T>;
public:
	MathPlane3() = default;
	MathPlane3(const Vec3& normal_, T distance_)
		: normal(normal_)
		, distance(distance_) 
	{}

	MathPlane3(const Vec3& normal_, const Vec3& point_)
		: normal(normal_)
		, distance(normal_.dot(point_))
	{}

	MathPlane3(const MathTriangle3<T>&  tri) {
		normal = (tri.v1 - tri.v0).cross(tri.v2 - tri.v0).normal();
		distance = normal.dot(tri.v0);
	}

	void onFormat(fmt::format_context& f) const;

	Vec3	normal;
	T		distance; // distance from origin
};

template<class T>
struct MathCylinder3 {
	using Vec3 = sge::Vec3<T>;
public:
	MathCylinder3() = default;
	MathCylinder3(const Vec3& start_, const Vec3& end_, const T& radius_)
		: start(start_)
		, end(end_)
		, radius(radius_)
	{}

	Vec3	start;
	Vec3	end;
	T		radius;
};

template<class T>
struct MathCapsule3 {
	using Vec3 = sge::Vec3<T>;
public:
	MathCapsule3() = default;
	MathCapsule3(const Vec3& start_, const Vec3& end_, const T& radius_)
		: start(start_)
		, end(end_)
		, radius(radius_)
	{}

	Vec3	start;
	Vec3	end;
	T		radius;
};


template<class T>
struct MathLine2 {
	using Vec2 = sge::Vec2<T>;
public:
	MathLine2() = default;
	MathLine2(const Vec2& start_, const Vec2& end_)
		: start(start_)
		, end(end_)
	{}

	bool getClosestPoint(Vec2& outPoint, const Vec2& inPoint) const;
	T	distanceToPoint(const Vec2& pt) const;

	Vec2	start;
	Vec2	end;
};

template<class T>
struct MathLine3 {
	using Vec3 = sge::Vec3<T>;
public:
	MathLine3() = default;
	MathLine3(const Vec3& start_, const Vec3& end_)
		: start(start_)
		, end(end_)
	{}

	bool getClosestPoint(Vec3& outPoint, const Vec3& inPoint) const;

	T	distanceToPoint(const Vec3& pt) const;

	Vec3	start;
	Vec3	end;
};

//-----------------------------------------
using MathxSphere3f = MathxSphere3<float>;
using MathxSphere3d = MathxSphere3<double>;

using MathPlane3f  = MathPlane3<float>;
using MathPlane3d  = MathPlane3<double>;

using MathTriangle3f = MathTriangle3<float>;
using MathTriangle3d = MathTriangle3<double>;

using MathCapsule3f = MathCapsule3<float>;
using MathCapsule3d = MathCapsule3<double>;

using MathLine2f = MathLine2<float>;
using MathLine2d = MathLine2<double>;

using MathLine3f = MathLine3<float>;
using MathLine3d = MathLine3<double>;
