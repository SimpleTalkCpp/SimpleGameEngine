#pragma once

#include "EditMesh.h"
#include "../Render_Common.h"
#include <sge_render/buffer/RenderGpuBuffer.h>

namespace sge {

class RenderMesh {
public:
	void create(const EditMesh& src);

	RenderGpuBuffer* vertexBuf() { return _vertexBuf; }
	size_t vertexCount() const { return _vertexCount; }
	const VertexLayout* layout() const { return _layout; }
	RenderPrimitiveType primitive() const { return _primitive; }

private:
	const VertexLayout*	_layout = nullptr;

	RenderPrimitiveType _primitive = RenderPrimitiveType::None;

	size_t _vertexCount = 0;
	SPtr<RenderGpuBuffer>	_vertexBuf;
};

}