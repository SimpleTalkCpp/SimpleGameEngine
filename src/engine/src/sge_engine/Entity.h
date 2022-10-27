#pragma once

#include <sge_engine/components/Component.h>

namespace sge {

enum class EntityId : u64 { None };
SGE_FORMATTER_ENUM_AS_INT(EntityId)

class Entity : public Object {
	SGE_OBJECT_TYPE(Entity, Object)
public:
	void setName(StrView name) { _name = name; }
	StrView name() const { return _name; }

	void		setId(EntityId id) { _id = id; }
	EntityId	id() const { return _id; }

	template<class C>
	C* addComponent() {
		auto* p = new C();
		_components.emplace_back(p);
		p->internal_setEntity(this);
		return p;
	}

	Span< SPtr<Component> >	components() { return _components; }

private:
	Vector< SPtr<Component> >	_components;

	String		_name;
	EntityId	_id = EntityId::None;
};

}
