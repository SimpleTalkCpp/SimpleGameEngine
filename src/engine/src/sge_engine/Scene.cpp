#include "Scene.h"

namespace sge {

Scene::Scene() {
	_rootEntity = addEntity();
	_rootEntity->transform()->_setIsRoot(true);
}

Scene::~Scene() {
}

Entity* Scene::addEntity() {
	auto* p  = new Entity();
	p->_internalInit(this, static_cast<EntityId>(++_nextEntityId));
	_entities.emplace_back(p);
	_entityIdMap.emplace(p->id(), p);

	if (_rootEntity) {
		_rootEntity->transform()->addChild(p->transform());
	}
	return p;
}

Entity* Scene::addEntity(StrView name) {
	auto p = addEntity();
	p->setName(name);
	return p;
}

void Scene::_internalOnEntityDestroy(Entity* e) {
	_entities.remove(e);
	_entityIdMap.erase(e->id());
}

}
