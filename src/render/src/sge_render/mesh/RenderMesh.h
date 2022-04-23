#pragma once

#include "EditMesh.h"
#include "../Render_Common.h"
#include <sge_render/buffer/RenderGpuBuffer.h>

namespace sge {

class RenderMesh {
public:
	void create(const EditMesh& src);

	RenderPrimitiveType primitive() const		{ return _primitive; }

	size_t vertexCount() const					{ return _vertexCount; }
	RenderGpuBuffer* vertexBuf()				{ return _vertexBuf; }
	const VertexLayout* vertexLayout() const	{ return _vertexLayout; }

private:
	RenderPrimitiveType _primitive = RenderPrimitiveType::None;

	size_t _vertexCount = 0;
	const VertexLayout*	_vertexLayout = nullptr;

	SPtr<RenderGpuBuffer>	_vertexBuf;
};

}