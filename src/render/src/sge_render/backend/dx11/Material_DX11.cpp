#include "Material_DX11.h"
#include "Renderer_DX11.h"
#include "RenderContext_DX11.h"

namespace sge {

void Material_DX11::bind(RenderContext_DX11* ctx, const VertexLayout* vertexLayout) {
	for (auto& pass : _passes) {
		if (pass) pass->bind(ctx, vertexLayout);
	}
}

void Material_DX11::onSetShader() {
	_passes.clear();

	auto* sh = shader();

	auto n = sh->passCount();

	for (size_t i = 0; i < n; i++) {
		auto* pass = new Pass(this, sh->getPass(i));
		_passes.emplace_back(pass);
	}
}

Material_DX11::Pass::Pass(Material_DX11* material, Shader_DX11::Pass* shPass)
	: _material(material)
	, _shPass(shPass)
{
	_vertexStage.reset(this, shPass->vertexStage());
	 _pixelStage.reset(this, shPass->pixelStage());
}

void Material_DX11::Pass::bind(RenderContext_DX11* ctx, const VertexLayout* vertexLayout) {
	_vertexStage.bind(ctx, vertexLayout);
	_pixelStage.bind(ctx, vertexLayout);
}

template<class SHADER_STAGE>
void Material_DX11::Stage<SHADER_STAGE>::reset(Pass* pass, SHADER_STAGE* shaderStage) {
	auto* renderer = Renderer_DX11::instance();

	_pass = pass;
	_shaderStage = shaderStage;

	auto* info = _shaderStage->info();

	auto constBufferCount = info->constBuffers.size();

	_constBuffers.clear();
	_constBuffers.reserve(constBufferCount);

	for (auto& cbInfo : info->constBuffers) {
		RenderGpuBuffer::CreateDesc desc;
		desc.type = RenderGpuBufferType::Const;
		desc.bufferSize = cbInfo.dataSize;

		auto cb = renderer->createGpuBuffer(desc);
		auto* p = static_cast<RenderGpuBuffer_DX11*>(cb.ptr());
		_constBuffers.emplace_back(p);
	}
}

void Material_DX11::PixelStage::bind(RenderContext_DX11* ctx, const VertexLayout* vertexLayout) {
	if (!_pass) return;
	if (!_shaderStage) return;
	_shaderStage->bind(ctx);
}

void Material_DX11::VertexStage::bind(RenderContext_DX11* ctx, const VertexLayout* vertexLayout) {
	if (!_pass) return;
	if (!_shaderStage) return;
	bindInputLayout(ctx, vertexLayout);
	_shaderStage->bind(ctx);
}

void Material_DX11::VertexStage::bindInputLayout(RenderContext_DX11* ctx, const VertexLayout* vertexLayout) {
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

		auto bytecode = _shaderStage->bytecode();

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



}