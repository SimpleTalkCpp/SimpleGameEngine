#pragma once

#include "Component.h"

namespace sge {

class CTransform : public Component {
	SGE_OBJECT_TYPE(CTransform, Component)
public:
	CTransform();

	void setLocalPos	(float x, float y, float z) { setLocalPos(Vec3f(x,y,z)); }
	void setLocalPos	(const Vec3f&  v) { _localPos = v;		_setLocalMatrixDirty(); }
	void setLocalRotate	(const Quat4f& v) { _localRotate = v;	_setLocalMatrixDirty(); }
	void setLocalScale	(const Vec3f&  v) { _localScale = v;	_setLocalMatrixDirty(); }

	const Vec3f&	localPos() const	{ return _localPos; }
	const Quat4f&	localRotate() const	{ return _localRotate; }
	const Vec3f&	localScale() const	{ return _localScale; }

	void addChild(CTransform* c);
	void removeChild(CTransform* c);

	CTransform*	getChild(size_t i) { return (i >= 0 && i < _children.size()) ? _children[i] : nullptr; }
	size_t childCount() const { return _children.size(); }

	const Mat4f& localMatrix();
	const Mat4f& worldMatrix();

	void _setIsRoot(bool b) { _isRoot = b; }
private:

	void _computeLocalMatrix();
	void _computeWorldMatrix();

	void _setLocalMatrixDirty();
	void _setWorldMatrixDirty();

	Vec3f	_localPos		{0,0,0};
	Quat4f	_localRotate	{0,0,0,0};
	Vec3f	_localScale		{1,1,1};

	struct Dirty {
		Dirty() 
			: localMatrix(true)
			, worldMatrix(true)
		{}
		bool	localMatrix : 1;
		bool	worldMatrix : 1;
	};

	bool _isRoot : 1;

	Dirty		_dirty;
	CTransform*	_parent = nullptr;
	Vector<CTransform*>		_children;

	Mat4f	_localMatrix;
	Mat4f	_worldMatrix;
};

inline
void CTransform::_setLocalMatrixDirty() {
	if (_dirty.localMatrix) return;
	_dirty.localMatrix = true;
	_setWorldMatrixDirty();
}

inline
const Mat4f& CTransform::localMatrix() {
	if (_dirty.localMatrix) {
		_dirty.localMatrix = false;
		_computeLocalMatrix();
	}
	return _localMatrix;
}

inline
const Mat4f& CTransform::worldMatrix() {
	if (_dirty.worldMatrix) {
		_dirty.worldMatrix = false;
		_computeWorldMatrix();
	}
	return _worldMatrix;
}

}