#pragma once

#include "Vertex.h"

namespace sge {


class VertexLayoutManager {
public:
	VertexLayoutManager();

	static VertexLayoutManager* current();

	const VertexLayout* getLayout(VertexType type);

	VertexLayout* _createLayout(VertexType type);

	template<class VERTEX>
	void registerLayout() {

		VertexLayout* layout = _createLayout(VERTEX::kType);
		VERTEX* v = nullptr;
		layout->stride = sizeof(VERTEX);
		layout->type = VERTEX::kType;
		v->onRegister(layout);
	}

private:
	static VertexLayoutManager* s_current;	
	Map<VertexType, VertexLayout>	_table;
};

}