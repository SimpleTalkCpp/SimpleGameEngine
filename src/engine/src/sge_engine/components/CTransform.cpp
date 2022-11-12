#include "CTransform.h"

namespace sge {

const TypeInfo* CTransform::s_getType() {
	class TI : public TI_Base {
	public:
		static void setLocalPos		(This& obj, const Vec3f&  v) { obj.setLocalPos(v); }
		static void setLocalRotate	(This& obj, const Quat4f& v) { obj.setLocalRotate(v); }
		static void setLocalScale	(This& obj, const Vec3f&  v) { obj.setLocalScale(v); }

		static const Vec3f&		getLocalPos		(const This& obj) { return obj.localPos(); }
		static const Quat4f&	getLocalRotate	(const This& obj) { return obj.localRotate(); }
		static const Vec3f&		getLocalScale	(const This& obj) { return obj.localScale(); }

		TI() {
			static FieldInfo fi[] = {
				{"position",	&This::_localPos,		getLocalPos,	setLocalPos},
				{"rotate",		&This::_localRotate,	getLocalRotate, setLocalRotate},
				{"scale",		&This::_localScale,		getLocalScale,	setLocalScale},
			};
			setFields(fi);
		}
	};
	static TI ti;
	return &ti;

}

CTransform::CTransform() {
	_isRoot = false;
}

void CTransform::addChild(CTransform* c) {
	if (c->_parent) {
		c->_parent->removeChild(c);
	}

	c->_parent = _isRoot ? nullptr : this;
	c->_setWorldMatrixDirty();
	_children.emplace_back(c);
}

void CTransform::removeChild(CTransform* c) {
	for (auto it = _children.begin(); it != _children.end(); ++it) {
		if (*it == c) {
			_children.erase(it);
			c->_parent = nullptr;
			c->_setWorldMatrixDirty();
			return;
		}
	}
}

void CTransform::_computeLocalMatrix() {
	_localMatrix = Mat4f::s_TRS(_localPos, _localRotate, _localScale);
}

void CTransform::_computeWorldMatrix() {
	if (_parent) {
		_worldMatrix = _parent->worldMatrix() * localMatrix();
	} else {
		_worldMatrix = localMatrix();
	}
}

void CTransform::_setWorldMatrixDirty() {
	if (_dirty.worldMatrix) return;
	_dirty.worldMatrix = true;
	for (auto* c : _children) {
		if (!c) continue;
		c->_setWorldMatrixDirty();
	}
}

}