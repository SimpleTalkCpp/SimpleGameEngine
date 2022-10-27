#include "EntitySelection.h"

namespace sge {

void EntitySelection::add(EntityId id) {
	if (has(id)) return;

	_entities.emplace_back(id);
	_entitiesSet.emplace(id);
}

void EntitySelection::select(EntityId id) {
	deselectAll();
	add(id);
}

void EntitySelection::deselectAll() {
	_entities.clear();
	_entitiesSet.clear();
}

bool EntitySelection::has(EntityId id) const {
	auto it = _entitiesSet.find(id);
	return it != _entitiesSet.end();
}

}