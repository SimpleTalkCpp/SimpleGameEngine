#pragma once

namespace sge {

class Entity;

class Component : public Object {
	SGE_OBJECT_TYPE(Component, Object)
public:

	void internal_setEntity(Entity* e) { _entity = e; }

private:
	Entity*	_entity = nullptr;

};

}
