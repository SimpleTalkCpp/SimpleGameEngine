#include "Material_DX11.h"
#include "Renderer_DX11.h"
#include "RenderContext_DX11.h"

namespace sge {

template<class STAGE>
void Material_DX11::_bindStageHelper(RenderContext_DX11* ctx, STAGE* stage) {
	auto* shaderStage = stage->shaderStage();
	if (!shaderStage) return;
	shaderStage->bind(ctx);

	auto* dc = ctx->renderer()->d3dDeviceContext();

	for (auto& cb : stage->constBuffers()) {
		cb.uploadToGpu();

		auto* cbInfo = cb.info();
		UINT bindPoint = cbInfo->bindPoint;
		
		auto* gpuBuffer = static_cast<RenderGpuBuffer_DX11*>(cb.gpuBuffer.ptr());
		if (!gpuBuffer) throw SGE_ERROR("cosnt buffer is null");

		auto* d3dBuf = gpuBuffer->d3dBuf();
		if (!d3dBuf) throw SGE_ERROR("d3dbuffer is null");

		stage->_dxSetConstBuffer(dc, bindPoint, d3dBuf);
	}
}

void Material_DX11::MyPixelStage::bind(RenderContext_DX11* ctx, const VertexLayout* vertexLayout) {
	_bindStageHelper(ctx, this);
}

void Material_DX11::MyVertexStage::bind(RenderContext_DX11* ctx, const VertexLayout* vertexLayout) {
	_bindStageHelper(ctx, this);

	bindInputLayout(ctx, vertexLayout);
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

		auto* vsInfo = info();
		for (auto& input : vsInfo->inputs) {
			auto* e = vertexLayout->find(input.semantic);
			if (!e) {
				throw SGE_ERROR("vertex sematic {} not found", input.semantic);
			}

			auto& dst = inputDesc.emplace_back();
			auto semanticType			= VertexSemanticUtil::getType(e->semantic);
			dst.SemanticName			= Util::getDxSemanticName(semanticType);
			dst.SemanticIndex			= VertexSemanticUtil::getIndex(e->semantic);
			dst.Format					= Util::getDxFormat(e->dataType);
			dst.InputSlot				= 0;
			dst.AlignedByteOffset		= e->offset;
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


Material_DX11::MyPass::MyPass(Material* material, ShaderPass* shaderPass) 
	: Pass(material, shaderPass)
	, _myVertexStage(this, shaderPass->vertexStage())
	, _myPixelStage (this, shaderPass->pixelStage())
{
	_vertexStage = &_myVertexStage;
	_pixelStage  = &_myPixelStage;
}

}