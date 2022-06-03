#include "Material_DX11.h"
#include "Renderer_DX11.h"
#include "RenderContext_DX11.h"

namespace sge {

#if 0

template<class SHADER_STAGE>
void Material_DX11::Stage<SHADER_STAGE>::setParam(StrView name, const ShaderPropValueConstPtr& valuePtr) {
	if (!_shaderStage) { SGE_ASSERT(false); return; }

	auto* info_ = info();
	size_t i = 0;
	for (auto& cb : info_->constBuffers) {
		SGE_ASSERT(i < _constBuffers.size());
		cb.setParamToBuffer(_constBuffers[i]._cpuBuffer, name, valuePtr);
		i++;
	}
}

template<class SHADER_STAGE>
void Material_DX11::Stage<SHADER_STAGE>::reset(Pass* pass, SHADER_STAGE* shaderStage) {
	auto* renderer = Renderer_DX11::instance();

	_pass = pass;
	_shaderStage = shaderStage;

	auto* info_ = info();
	auto constBufferCount = info_->constBuffers.size();

	_constBuffers.clear();
	_constBuffers.reserve(constBufferCount);

	for (auto& cbInfo : info_->constBuffers) {
		RenderGpuBuffer::CreateDesc desc;
		desc.type = RenderGpuBufferType::Const;
		desc.bufferSize = cbInfo.dataSize;

		auto& dst = _constBuffers.emplace_back();
		auto gpuBuf = renderer->createGpuBuffer(desc);
		dst._gpuBuffer = static_cast<RenderGpuBuffer_DX11*>(gpuBuf.ptr());
	}
}
#endif

void Material_DX11::MyPixelStage::bind(RenderContext_DX11* ctx, const VertexLayout* vertexLayout) {
	if (!_pass) return;
	auto* ss = static_cast<Shader_DX11::MyPixelStage*>(_shaderStage);
	if (!ss) return;
	ss->bind(ctx);
}

void Material_DX11::MyVertexStage::bind(RenderContext_DX11* ctx, const VertexLayout* vertexLayout) {
	if (!_pass) return;
	if (!_shaderStage) return;
	bindInputLayout(ctx, vertexLayout);
	auto* ss = static_cast<Shader_DX11::MyVertexStage*>(_shaderStage);
	ss->bind(ctx);
}

void Material_DX11::MyVertexStage::bindInputLayout(RenderContext_DX11* ctx, const VertexLayout* vertexLayout) {
	if (!vertexLayout) throw SGE_ERROR("vertexLayout is null");

	auto* dev = ctx->renderer()->d3dDevice();
	auto* dc  = ctx->renderer()->d3dDeviceContext();


	DX11_ID3DInputLayout* dxLayout = nullptr;

	auto it = _inputLayoutsMap.find(vertexLayout);
	if (it != _inputLayoutsMap.end()) {
		dxLayout = it->second;
	} else {
		Vector_<D3D11_INPUT_ELEMENT_DESC, 32> inputDesc;

		for (auto& e : vertexLayout->elements) {
			auto& dst = inputDesc.emplace_back();
			auto semanticType			= VertexSemanticUtil::getType(e.semantic);
			dst.SemanticName			= Util::getDxSemanticName(semanticType);
			dst.SemanticIndex			= VertexSemanticUtil::getIndex(e.semantic);
			dst.Format					= Util::getDxFormat(e.dataType);
			dst.InputSlot				= 0;
			dst.AlignedByteOffset		= e.offset;
			dst.InputSlotClass			= D3D11_INPUT_PER_VERTEX_DATA;
			dst.InstanceDataStepRate	= 0;
		}

		ComPtr<DX11_ID3DInputLayout>	outLayout;

		auto* ss = static_cast<Shader_DX11::MyVertexStage*>(_shaderStage);
		auto bytecode = ss->bytecode();

		auto hr = dev->CreateInputLayout(inputDesc.data()
										, static_cast<UINT>(inputDesc.size())
										, bytecode.data()
										, bytecode.size()
										, outLayout.ptrForInit());
		Util::throwIfError(hr);

		dxLayout = outLayout;
		_inputLayoutsMap[vertexLayout] = std::move(outLayout);
	}

	dc->IASetInputLayout(dxLayout);
}

void Material_DX11::MyPass::onBind(RenderContext* ctx_, const VertexLayout* vertexLayout) {
	auto* ctx = static_cast<RenderContext_DX11*>(ctx_);
	_myVertexStage.bind(ctx, vertexLayout);
	 _myPixelStage.bind(ctx, vertexLayout);
}

//void Material_DX11::MyPass::setParam(StrView name, const ShaderPropValueConstPtr& valuePtr) {
//	_myVertexStage.setParam(name, valuePtr);
//	 _myPixelStage.setParam(name, valuePtr);
//}

Material_DX11::MyPass::MyPass(Material* material, ShaderPass* shaderPass) 
	: Pass(material, shaderPass)
	, _myVertexStage(this, shaderPass->vertexStage())
	, _myPixelStage (this, shaderPass->pixelStage())
{
	_vertexStage = &_myVertexStage;
	_pixelStage  = &_myPixelStage;
}

}