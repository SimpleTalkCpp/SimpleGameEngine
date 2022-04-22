#include "VertexLayoutManager.h"

namespace sge {

VertexLayoutManager* VertexLayoutManager::s_current = nullptr;

VertexLayoutManager::VertexLayoutManager() {
	registerLayout<Vertex_Pos>();
	registerLayout<Vertex_PosColor>();
	registerLayout<Vertex_PosUvColor>();
	registerLayout<Vertex_PosUv2Color>();
}

VertexLayoutManager* VertexLayoutManager::current() {
	static VertexLayoutManager s;
	return &s;
}

const VertexLayout* VertexLayoutManager::get(VertexType type) {
	auto it = _table.find(type);
	if (it == _table.end()) {
		return nullptr;
	}
	return &it->second;
}

VertexLayout* VertexLayoutManager::_createLayout(VertexType type) {
	auto* p = get(type);
	if (p) {
		SGE_ASSERT(false);
	}
	return &_table[type];
}

}