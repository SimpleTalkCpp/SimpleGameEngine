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
	ComPtr<DX11_IDXGISwapChain>			_swapChain;
	ComPtr<DX11_ID3DRenderTargetView>	_renderTargetView;
	ComPtr<DX11_ID3DTexture2D>			_depthStencil;
	ComPtr<DX11_ID3DDepthStencilView>	_depthStencilView;

};

}