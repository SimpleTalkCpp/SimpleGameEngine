#pragma once

#include "EditMesh.h"
#include "../Render_Common.h"
#include <sge_render/buffer/RenderGpuBuffer.h>

namespace sge {

class RenderSubMesh {
	SPtr<RenderGpuBuffer>	_vertexBuf;
	SPtr<RenderGpuBuffer>	_indexBuf;
};

class RenderMesh {
public:
	using SubMesh = RenderSubMesh;
	void create(const EditMesh& src);

	RenderPrimitiveType primitive() const		{ return _primitive; }

	size_t vertexCount() const					{ return _vertexCount; }
	RenderGpuBuffer* vertexBuf()				{ return _vertexBuf; }
	const VertexLayout* vertexLayout() const	{ return _vertexLayout; }

private:
	
	RenderPrimitiveType _primitive = RenderPrimitiveType::None;

	size_t _vertexCount = 0;
	const VertexLayout*	_vertexLayout = nullptr;
	RenderDataType _indexType = RenderDataType::UInt16;

	// Vector_<SubMesh, 1>	_subMeshes;
	SPtr<RenderGpuBuffer>	_vertexBuf;
};

}