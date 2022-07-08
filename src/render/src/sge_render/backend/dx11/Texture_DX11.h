#pragma once

#include <sge_render/textures/Texture.h>
#include <sge_render/backend/dx11/Render_DX11_Common.h>

namespace sge {

class Texture2D_DX11 : public Texture2D {
	using Base = Texture2D;
	using Util = DX11Util;
public:
	Texture2D_DX11(CreateDesc& desc);

	DX11_ID3DShaderResourceView*	resourceView() { return _resourceView.ptr(); }
	DX11_ID3DSamplerState*			samplerState() { return _samplerState.ptr(); }

private:
	ComPtr<DX11_ID3DTexture2D>			_tex;
	ComPtr<DX11_ID3DShaderResourceView>	_resourceView;
	ComPtr<DX11_ID3DSamplerState>		_samplerState;
};

} // namespace
