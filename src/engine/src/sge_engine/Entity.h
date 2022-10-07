#pragma once

#include <sge_engine/components/Component.h>

namespace sge {

enum EntityId : u64 { None = 0 };

class Entity : public Object {
public:
	Entity(EntityId id) { _id = id; }

	void setName(StrView name) { _name = name; }
	StrView name() const { return _name; }

	EntityId	id() const { return _id; }

	template<class C>
	C* addComponent() {
		auto* p = new C();
		_components.emplace_back(p);
		return p;
	}

private:
	Vector< SPtr<Component> >	_components;

	String		_name;
	EntityId	_id;
};

}
