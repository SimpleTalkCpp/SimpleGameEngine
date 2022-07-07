#pragma once

#include <sge_render/shader/Material.h>
#include "Shader_DX11.h"
#include "RenderGpuBuffer_DX11.h"

namespace sge {

class Material_DX11 : public Material {
public:

private:
	using Util = DX11Util;
	Shader_DX11* shader() { return static_cast<Shader_DX11*>(_shader.ptr()); }

	template<class STAGE>
	static void _bindStageHelper(RenderContext_DX11* ctx, STAGE* stage);

	struct MyVertexStage : public VertexStage {
		using Base = VertexStage;

		MyVertexStage(MaterialPass* pass, ShaderVertexStage* shaderStage) : Base(pass, shaderStage) {}

		void bind(RenderContext_DX11* ctx, const VertexLayout* vertexLayout);
		void bindInputLayout(RenderContext_DX11* ctx, const VertexLayout* vertexLayout);

		void _dxSetConstBuffer(DX11_ID3DDeviceContext* dc, UINT bindPoint, DX11_ID3DBuffer* d3dBuf) {
			dc->VSSetConstantBuffers(bindPoint, 1, &d3dBuf);
		}

		Span<ConstBuffer> constBuffers() { return _constBuffers; }
		Shader_DX11::MyVertexStage*	shaderStage() { return static_cast<Shader_DX11::MyVertexStage*>(_shaderStage); }

		VectorMap<const VertexLayout*, ComPtr<DX11_ID3DInputLayout>> _inputLayoutsMap;
	};

	struct MyPixelStage : public PixelStage {
		using Base = PixelStage;
		using MyShaderStage = Shader_DX11::MyPixelStage;

		MyPixelStage(MaterialPass* pass, ShaderPixelStage* shaderStage) : Base(pass, shaderStage) {}
		void bind(RenderContext_DX11* ctx, const VertexLayout* vertexLayout);

		void _dxSetConstBuffer(DX11_ID3DDeviceContext* dc, UINT bindPoint, DX11_ID3DBuffer* d3dBuf) {
			dc->PSSetConstantBuffers(bindPoint, 1, &d3dBuf);
		}

		Span<ConstBuffer> constBuffers() { return _constBuffers; }
		Shader_DX11::MyPixelStage*	shaderStage() { return static_cast<Shader_DX11::MyPixelStage*>(_shaderStage); }
	};

	struct MyPass : public Pass {
		MyPass(Material* material, ShaderPass* shaderPass);

		virtual void onBind(RenderContext* ctx, const VertexLayout* vertexLayout) override;

		void _bindRenderState(RenderContext_DX11* ctx);

		MyVertexStage _myVertexStage;
		MyPixelStage  _myPixelStage;

		ComPtr<DX11_ID3DRasterizerState>	_rasterizerState;
		ComPtr<DX11_ID3DDepthStencilState>	_depthStencilState;
		ComPtr<DX11_ID3DBlendState>			_blendState;
	};

	virtual Pass* onCreatePass(Material* material, ShaderPass* shaderPass) override {
		return new MyPass(material, shaderPass);
	}
};

}