#pragma once

#include <sge_engine/components/Component.h>

namespace sge {

class Scene;
class CTransform;

enum class EntityId : u64 { None };
SGE_FORMATTER_ENUM_AS_INT(EntityId)

class Entity : public Object {
	SGE_OBJECT_TYPE(Entity, Object)
public:
	Entity();

	void setName(StrView name) { _name = name; }
	StrView name() const { return _name; }

	void		_internalInit(Scene* scene, EntityId id) { _scene = scene; _id = id; }
	
	Scene*		scene() const { return _scene; }
	EntityId	id() const { return _id; }

	template<class C>
	C* addComponent() {
		auto* p = new C();
		_components.emplace_back(p);
		p->_internalSetEntity(this);
		return p;
	}

	template<class C>
	C* getComponent() {
		auto* type = TypeOf<C>();
		for (auto* c : _components) {
			if (c->getType() == type) {
				return c;
			}
		}
		return nullptr;
	}

	Span< SPtr<Component> >	components() { return _components; }

	CTransform*	transform() { return _transform.ptr(); }

private:
	Vector< SPtr<Component> >	_components;

	String				_name;
	Scene*				_scene = nullptr;
	EntityId			_id = EntityId::None;
	SPtr<CTransform>	_transform = nullptr;
};

}
