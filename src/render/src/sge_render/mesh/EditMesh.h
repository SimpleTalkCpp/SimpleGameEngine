#pragma once

#include "../vertex/Vertex.h"

namespace sge {

class EditMesh : public NonCopyable {
public:
	static const u8 kUvCountMax = 4;
	
	Vector<Tuple3f>	pos;
	Vector<Tuple2f>	uv[kUvCountMax];
	Vector<Color4b> color;

	Vector<Tuple3f>	normal;
	Vector<Tuple3f>	tangent;
	Vector<Tuple3f>	binormal;

	void loadObjFile(StrView filename);
};

}