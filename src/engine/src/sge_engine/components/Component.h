#pragma once

namespace sge {

class Entity;

class Component : public Object {
	SGE_OBJECT_TYPE(Component, Object)
public:

	void _internalSetEntity(Entity* e) { _entity = e; }

	Entity*	entity() { return _entity; }

private:
	Entity*	_entity = nullptr;

};

}
