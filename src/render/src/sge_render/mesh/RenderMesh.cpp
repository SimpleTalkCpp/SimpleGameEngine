#include "RenderMesh.h"
#include <sge_render/Renderer.h>
#include <sge_render/vertex/VertexLayoutManager.h>

namespace sge {

template<class DST, class T> inline
void RenderMesh_copyVertexData(DST* dst, size_t vertexCount, const VertexLayout::Element& element, size_t stride, const T* src) {
	u8* p = dst + element.offset;

	for (size_t i = 0; i < vertexCount; i++) {
		*reinterpret_cast<T*>(p) = *src;
		src++;
		p += stride;
	}
}

void RenderMesh::create(const EditMesh& src) {
	u8 uvCount = 0;
	u8 colorCount = 0;
	u8 normalCount = 0;

	_primitive = RenderPrimitiveType::Triangles;
	_vertexCount = src.pos.size();
	if (_vertexCount <= 0)
		return;

	if (src.color.size() > 0) {
		if (src.color.size() < _vertexCount) {
			SGE_ASSERT(false);
		} else {
			colorCount = 1;
		}
	}

	for (u8 i = 0; i < EditMesh::kUvCountMax; i++) {
		auto& uv = src.uv[i];
		if (uv.size() > 0) {
			if (uv.size() <= _vertexCount) {
				SGE_ASSERT(false);
			} else {
				uvCount = i + 1;
			}
		}
	}

	auto vertexType = VertexType_make(
						RenderDataType_get<Tuple3f>(), 
						RenderDataType_get<Color4b>(), colorCount, 
						RenderDataType_get<Tuple2f>(), uvCount,
						RenderDataType_get<Tuple3f>(), normalCount, 0, 0);

	_vertexLayout = VertexLayoutManager::current()->getLayout(vertexType);

	if (!_vertexLayout) {
		throw SGE_ERROR("cannot find vertex Layout for mesh");
	}

//------
	Vector_<u8, 1024>	vertexData;
	vertexData.resize(_vertexLayout->stride * _vertexCount);

	{
		auto* data = vertexData.data();
		auto stride = _vertexLayout->stride;

		for (auto& element : _vertexLayout->elements) {
			switch (element.semantic) {
				case VertexLayout_Semantic::Pos: {
					RenderMesh_copyVertexData(data, _vertexCount, element, stride, src.pos.data());
				} break;
				case VertexLayout_Semantic::Color0: {
					RenderMesh_copyVertexData(data, _vertexCount, element, stride, src.color.data());
				} break;
				case VertexLayout_Semantic::TexCoord0: {
					RenderMesh_copyVertexData(data, _vertexCount, element, stride, src.uv[0].data());
				} break;
				case VertexLayout_Semantic::TexCoord1: {
					RenderMesh_copyVertexData(data, _vertexCount, element, stride, src.uv[1].data());
				} break;
				case VertexLayout_Semantic::TexCoord2: {
					RenderMesh_copyVertexData(data, _vertexCount, element, stride, src.uv[2].data());
				} break;
				case VertexLayout_Semantic::TexCoord3: {
					RenderMesh_copyVertexData(data, _vertexCount, element, stride, src.uv[3].data());
				} break;
			}
		}
	}

//------
	auto* renderer = Renderer::current();
	{
		RenderGpuBuffer::CreateDesc desc;
		desc.type = RenderGpuBufferType::Vertex;
		desc.bufferSize = _vertexCount * _vertexLayout->stride;
		_vertexBuf = renderer->createGpuBuffer(desc);

		_vertexBuf->uploadToGpu(vertexData);
	}

}

}