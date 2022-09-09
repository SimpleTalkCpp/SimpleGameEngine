#include "VertexLayoutManager.h"

namespace sge {

VertexLayoutManager::VertexLayoutManager() {
	registerLayout<Vertex_Pos2f>();
	registerLayout<Vertex_ImGui>();

	registerLayout<Vertex_Pos>();
	registerLayout<Vertex_PosNormal>();

	registerLayout<Vertex_PosColor>();
	registerLayout<Vertex_PosColorNormal>();

	#define MY_REGISTER_VERTEX_UV(T) \
		registerLayout<T<1>>(); \
		registerLayout<T<2>>(); \
		registerLayout<T<3>>(); \
		registerLayout<T<4>>(); \
	//---
		MY_REGISTER_VERTEX_UV(Vertex_PosUv);
		MY_REGISTER_VERTEX_UV(Vertex_PosColorUv);

		MY_REGISTER_VERTEX_UV(Vertex_PosNormalUv);
		MY_REGISTER_VERTEX_UV(Vertex_PosColorNormalUv);

		MY_REGISTER_VERTEX_UV(Vertex_PosTangentUv);
		MY_REGISTER_VERTEX_UV(Vertex_PosColorTangentUv);

		MY_REGISTER_VERTEX_UV(Vertex_PosBinormalUv);
		MY_REGISTER_VERTEX_UV(Vertex_PosColorBinormalUv);

	#undef MY_REGISTER_VERTEX_UV

	registerLayout<Vertex_VertexId>();

}

VertexLayoutManager* VertexLayoutManager::instance() {
	static VertexLayoutManager s;
	return &s;
}

const VertexLayout* VertexLayoutManager::getLayout(VertexType type) {
	auto it = _table.find(type);
	if (it == _table.end()) {
		return nullptr;
	}
	return &it->second;
}

VertexLayout* VertexLayoutManager::_createLayout(VertexType type) {
	auto* p = getLayout(type);
	if (p) {
		throw SGE_ERROR("duplicated layout");
	}
	return &_table[type];
}

}