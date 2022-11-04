#include "Frustum3.h"

namespace sge {
namespace Math {

template<class T>
void Frustum3<T>::updatePlanesFromPoints() {
	  nearPlane().setByTriangle(points[0], points[1], points[2]);
	   farPlane().setByTriangle(points[4], points[6], points[5]);

	  leftPlane().setByTriangle(points[0], points[3], points[4]);
	 rightPlane().setByTriangle(points[1], points[5], points[2]);

	   topPlane().setByTriangle(points[2], points[6], points[3]);
	bottomPlane().setByTriangle(points[0], points[4], points[1]);
}

template<class T>
bool Frustum3<T>::isOverlapped(const Frustum3& f) const {
	for (size_t i = 0; i < 6; i++) {
		if (_outsideOfPlane(planes[i], f.points))
			return false;
	}

	for (size_t i = 0; i < 6; i++) {
		if (_outsideOfPlane(f.planes[i], points))
			return false;
	}

	return true;
}

template<class T>
void Frustum3<T>::setByBBox(const BBox3& bbox, const Mat4& matrix) {
	bbox.getPoints(points, matrix);
	updatePlanesFromPoints();
}

template<class T>
void Frustum3<T>::setByInvViewProjMatrix(const Mat4& invMatrix) {
	points[0] = invMatrix.mulPoint(Vec4(-1,-1, 0, 1)).toVec3();
	points[1] = invMatrix.mulPoint(Vec4( 1,-1, 0, 1)).toVec3();
	points[2] = invMatrix.mulPoint(Vec4( 1, 1, 0, 1)).toVec3();
	points[3] = invMatrix.mulPoint(Vec4(-1, 1, 0, 1)).toVec3();

	points[4] = invMatrix.mulPoint(Vec4(-1,-1, 1, 1)).toVec3();
	points[5] = invMatrix.mulPoint(Vec4( 1,-1, 1, 1)).toVec3();
	points[6] = invMatrix.mulPoint(Vec4( 1, 1, 1, 1)).toVec3();
	points[7] = invMatrix.mulPoint(Vec4(-1, 1, 1, 1)).toVec3();

	updatePlanesFromPoints();
}


template struct Frustum3<float>;
template struct Frustum3<double>;

} // namespace Math
} // namespace sge
