#include "MathCamera3.h"

namespace sge {
namespace Math {

template<class T>
void Camera3<T>::pan(T x, T y) {
	auto v = _aim - _pos;
	auto right = _up.cross(v).normalize();

	auto q = Quat4::s_eulerY(x) * Quat4::s_angleAxis(y, right);
	v    = q * v;
	_up  = q * _up;
	_aim = _pos + v;
}

template<class T>
void Camera3<T>::orbit(T x, T y) {
	auto v = _pos - _aim;
	auto right = _up.cross(v).normalize();

	auto q = Quat4::s_angleAxis(y, right) * Quat4::s_eulerY(x);
	v    = q * v;
	_up  = q * _up;
	_pos = _aim + v;
}

template<class T>
void Camera3<T>::move(T x, T y, T z) {
	auto v = _aim - _pos;
	auto dir = v.normalize();
	auto right = _up.cross(dir);

	auto t = right * x + _up * y + dir * z;
	_pos += t;
	_aim += t;
}

template<class T>
void Camera3<T>::dolly(T z) {
	auto v = _pos - _aim;
	auto dir = v.normalize();
	auto d = v.length();
	d += z;
	d = Math::max(d, static_cast<T>(0.001));
	
	_pos = _aim + dir * d;
}

template<class T>
Ray3<T> Camera3<T>::getRay(const Vec2& screenPos) const {
	return Ray3::s_unprojectFromInvMatrix(screenPos, viewProjMatrix().inverse(), _viewport);
}

template<class T>
Mat4<T> Camera3<T>::viewMatrix() const {
	return Mat4::s_lookAt(_pos, _aim, _up);
}

template<class T>
Mat4<T> Camera3<T>::projMatrix() const {
	T aspect = _viewport.h != 0 ? _viewport.w / _viewport.h : T(0);
	return Mat4::s_perspective(Math::radians(_fov), aspect, _nearClip, _farClip);
}

template struct Camera3<float>;
template struct Camera3<double>;

} // namespace Math
} // namespace sge
