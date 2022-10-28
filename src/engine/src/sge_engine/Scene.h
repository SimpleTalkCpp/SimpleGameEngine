#pragma once

#include "Entity.h"

namespace sge {

class Scene : public Object {
public:
	Scene();
	~Scene();

	Entity*		addEntity();
	Entity*		addEntity(StrView name);

	Span<Entity*>	entities() { return _entities; }

	Entity*		findEntityById(EntityId id);

	void		_internalOnEntityDestroy(Entity* e);

	Entity*		rootEntity() { return _rootEntity.ptr(); }

private:
	u64	_nextEntityId = 0;
	Vector<Entity*>	_entities;
	Map<EntityId, SPtr<Entity>>	_entityIdMap;

	SPtr<Entity>	_rootEntity;
};

inline Entity* Scene::findEntityById(EntityId id) {
	auto it = _entityIdMap.find(id);
	if (it == _entityIdMap.end()) return nullptr;
	return it->second;
}

}
