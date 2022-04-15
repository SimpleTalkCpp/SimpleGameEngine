#pragma once

#include "../../RenderContext.h"
#include "Render_DX11_Common.h"

namespace sge {

class RenderContext_DX11 : public RenderContext {
	using Base = RenderContext;
	using Util = DX11Util;
public:
	RenderContext_DX11(CreateDesc& desc);

protected:
	Renderer_DX11*	_renderer = nullptr;

	ComPtr<DX11_IDXGISwapChain>			_swapChain;
	ComPtr<DX11_ID3DRenderTargetView>	_renderTargetView;
	ComPtr<DX11_ID3DTexture2D>			_depthStencil;
	ComPtr<DX11_ID3DDepthStencilView>	_depthStencilView;

	ComPtr<DX11_ID3DBuffer>			_testVertexBuffer;
	ComPtr<DX11_ID3DVertexShader>	_testVertexShader;
	ComPtr<DX11_ID3DPixelShader>	_testPixelShader;
	ComPtr<DX11_ID3DInputLayout>	_testInputLayout;

	void _createRenderTarget();

	virtual void onBeginRender() override;
	virtual void onEndRender() override;

	virtual void onClearColorAndDepthBuffer() override;
	virtual void onSwapBuffers() override;
};

}