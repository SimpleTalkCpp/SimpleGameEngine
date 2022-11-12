#include "MathGeometry.h"
#include "BBox3.h"

namespace sge {
namespace Math {

template<class T>
struct Frustum3 {
	using Vec3	 = Vec3<T>;
	using Vec4	 = Vec4<T>;
	using Mat4   = Mat4<T>;
	using Plane3 = Plane3<T>;
	using BBox3  = BBox3<T>;
public:
	enum Side
	{
		Side_Near,
		Side_Far,
		Side_Left,
		Side_Right,
		Side_Top,
		Side_Bottom,
		Side__COUNT
	}; 

	Vec3	points[8];
	Plane3	planes[Side__COUNT];

	void setByViewProjMatrix(const Mat4& matrix) { setByInvViewProjMatrix(matrix.inverse()); }
	void setByInvViewProjMatrix(const Mat4& invMatrix);

	void setByBBox(const BBox3& bbox, const Mat4& matrix);

	bool isOverlapped(const BBox3& bbox, const Mat4& matrix) const;
	bool isOverlapped(const Frustum3& f) const;

	Plane3&	nearPlane	()	{ return planes[Side_Near  ]; }
	Plane3&	farPlane	()	{ return planes[Side_Far   ]; }
	Plane3&	leftPlane	()	{ return planes[Side_Left  ]; }
	Plane3&	rightPlane	()	{ return planes[Side_Right ]; }
	Plane3&	topPlane	()	{ return planes[Side_Top   ]; }
	Plane3&	bottomPlane	()	{ return planes[Side_Bottom]; }

	const Plane3&	nearPlane	() const { return planes[Side_Near  ]; }
	const Plane3&	farPlane	() const { return planes[Side_Far   ]; }
	const Plane3&	leftPlane	() const { return planes[Side_Left  ]; }
	const Plane3&	rightPlane	() const { return planes[Side_Right ]; }
	const Plane3&	topPlane	() const { return planes[Side_Top   ]; }
	const Plane3&	bottomPlane	() const { return planes[Side_Bottom]; }

private:
	void updatePlanesFromPoints();
	static bool _outsideOfPlane(const Plane3& p, const Vec3 points[8]);
};

template<class T> inline
bool Frustum3<T>::_outsideOfPlane(const Plane3& p, const Vec3 points[8]) {
	for (size_t i = 0; i < 8; i++) {
		if (p.dot(points[i]) < 0)
			return false;
	}
	return true;
}

template<class T> inline
bool Frustum3<T>::isOverlapped(const BBox3& bbox, const Mat4& matrix) const {
	Frustum3 tmp;
	tmp.setByBBox(bbox, matrix);
	return isOverlapped(tmp);
}

using Frustum3f = Frustum3<float>;
using Frustum3d = Frustum3<double>;

} // namespace Math
} // namespace sge