#pragma once

#include "Render_Common.h"
#include "vertex/Vertex.h"
#include "buffer/RenderGpuBuffer.h"

namespace sge {

class RenderDrawCall : public NonCopyable {
public:
	
	RenderPrimitiveType primitive = RenderPrimitiveType::None;
	const VertexLayout*	layout = nullptr;
	SPtr<RenderGpuBuffer>	vertexBuffer;
	size_t vertexCount = 0;
};

}