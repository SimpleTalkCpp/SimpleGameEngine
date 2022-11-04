#pragma once

#include "../vertex/Vertex.h"

namespace sge {

class EditMesh : public NonCopyable {
public:
	static const u8 kUvCountMax = 4;

	RenderPrimitiveType	primitive = RenderPrimitiveType::Triangles;
	Vector<u32>	indices;
	
	Vector<Tuple3f>	pos;
	Vector<Tuple2f>	uv[kUvCountMax];
	Vector<Color4b> color;

	Vector<Tuple3f>	normal;
	Vector<Tuple3f>	tangent;
	Vector<Tuple3f>	binormal;

	void addColors(const Color4b& c);

	void clear();
};

}