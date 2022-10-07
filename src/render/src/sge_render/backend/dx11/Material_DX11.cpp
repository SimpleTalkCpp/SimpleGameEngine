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

	for (auto& texParam : stage->texParams()) {
		auto* tex = texParam.getUpdatedTexture();

		int bindPoint = texParam.bindPoint();

		switch (texParam.dataType()) {
			case RenderDataType::Texture2D: {
				auto* tex2d = static_cast<Texture2D_DX11*>(tex);
				auto* rv = tex2d->resourceView();
				auto* ss = tex2d->samplerState();

				stage->_dxSetShaderResource(dc, bindPoint, rv);
				stage->_dxSetSampler(dc, bindPoint, ss);
			} break;
			
			default: throw SGE_ERROR("bind unsupported texture type");
		}
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
		Vector<D3D11_INPUT_ELEMENT_DESC, 32> inputDesc;

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

	_bindRenderState(ctx);
}

void Material_DX11::MyPass::_bindRenderState(RenderContext_DX11* ctx) {
	auto* renderer = ctx->renderer();

	if (!_rasterizerState) {
		HRESULT hr;
		auto* dev = renderer->d3dDevice();
		auto& rs = info()->renderState;

		D3D11_RASTERIZER_DESC rasterDesc = {};
		rasterDesc.AntialiasedLineEnable = true;
		rasterDesc.CullMode = Util::getDxCullMode(rs.cull);
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;

		rasterDesc.FillMode = rs.wireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
//		rasterDesc.FillMode = D3D11_FILL_WIREFRAME; // Test

		rasterDesc.FrontCounterClockwise = true;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = true;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		hr = dev->CreateRasterizerState(&rasterDesc, _rasterizerState.ptrForInit());
		Util::throwIfError(hr);
	}

	if (!_depthStencilState) {
		HRESULT hr;
		auto* dev = renderer->d3dDevice();
		auto& rs = info()->renderState;

		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};

		bool depthTest = rs.depthTest.isEnable();
		if (depthTest) {
			depthStencilDesc.DepthEnable	= true;
			depthStencilDesc.DepthFunc		= Util::getDxDepthTestOp(rs.depthTest.op);
		} else {
			depthStencilDesc.DepthEnable	= false;
			depthStencilDesc.DepthFunc		= D3D11_COMPARISON_ALWAYS;
		}

		depthStencilDesc.DepthWriteMask	= rs.depthTest.writeMask ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;

		depthStencilDesc.StencilEnable		= false;
		depthStencilDesc.StencilReadMask	= 0xFF;
		depthStencilDesc.StencilWriteMask	= 0xFF;

		hr = dev->CreateDepthStencilState(&depthStencilDesc, _depthStencilState.ptrForInit());
		Util::throwIfError(hr);
	}

	if (!_blendState) {
		HRESULT hr;
		auto* dev = renderer->d3dDevice();
		auto& rs = info()->renderState;

		D3D11_BLEND_DESC blendStateDesc = {};
		blendStateDesc.AlphaToCoverageEnable  = false;
		blendStateDesc.IndependentBlendEnable = false;
		auto& rtDesc = blendStateDesc.RenderTarget[0];

		rtDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		bool blendEnable = rs.blend.isEnable();
		if (blendEnable) {
			rtDesc.BlendEnable	  = true;

			if (rs.blend.rgb.op == RenderState_BlendOp::Disable) {
				rtDesc.BlendOp			= D3D11_BLEND_OP_ADD;
				rtDesc.SrcBlend			= D3D11_BLEND_ONE;
				rtDesc.DestBlend		= D3D11_BLEND_ZERO;
			} else {
				rtDesc.BlendOp			= Util::getDxBlendOp(rs.blend.rgb.op);
				rtDesc.SrcBlend			= Util::getDxBlendFactor(rs.blend.rgb.srcFactor);
				rtDesc.DestBlend		= Util::getDxBlendFactor(rs.blend.rgb.dstFactor);
			}

			if (rs.blend.alpha.op == RenderState_BlendOp::Disable) {
				rtDesc.BlendOpAlpha		= D3D11_BLEND_OP_ADD;
				rtDesc.SrcBlendAlpha	= D3D11_BLEND_ONE;
				rtDesc.DestBlendAlpha	= D3D11_BLEND_ZERO;
			} else {
				rtDesc.BlendOpAlpha		= Util::getDxBlendOp(rs.blend.alpha.op);
				rtDesc.SrcBlendAlpha	= Util::getDxBlendFactor(rs.blend.alpha.srcFactor);
				rtDesc.DestBlendAlpha	= Util::getDxBlendFactor(rs.blend.alpha.dstFactor);
			}
		}else{
			rtDesc.BlendEnable	  = false;
		}

		hr = dev->CreateBlendState(&blendStateDesc, _blendState.ptrForInit());
		Util::throwIfError(hr);
	}

	auto* d3dCtx = renderer->d3dDeviceContext();

	d3dCtx->RSSetState(_rasterizerState);
	d3dCtx->OMSetDepthStencilState(_depthStencilState, 1);
	
	Color4f blendColor(1,1,1,1);
	d3dCtx->OMSetBlendState(_blendState, blendColor.data, 0xffffffff);
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