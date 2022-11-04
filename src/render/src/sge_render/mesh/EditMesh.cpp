#include "EditMesh.h"

namespace sge {

void EditMesh::addColors(const Color4b& c) {
	for (size_t i = color.size(); i < pos.size(); i++) {
		color.emplace_back(c);
	}
}

void EditMesh::clear() {
	indices.clear();

	pos.clear();
	for (auto& t : uv) {
		t.clear();
	}
	color.clear();
	normal.clear();
	tangent.clear();
	binormal.clear();
}

}