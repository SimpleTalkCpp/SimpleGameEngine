#include "Scene.h"

namespace sge {

Entity* Scene::addEntity() {
	auto* p  = new Entity();
	p->setId(static_cast<EntityId>(++_nextEntityId));
	_entities.emplace_back(p);
	_entityIdMap.emplace(p->id(), p);
	return p;
}

Entity* Scene::addEntity(StrView name) {
	auto p = addEntity();
	p->setName(name);
	return p;
}

}
