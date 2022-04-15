#pragma once

#if SGE_RENDER_HAS_DX11

#include <sge_render/Renderer.h>
#include "Render_DX11_Common.h"

namespace sge {

class Renderer_DX11 : public Renderer {
	using Base = Renderer;
	using Util = DX11Util;
public:
	static Renderer_DX11* current() { return static_cast<Renderer_DX11*>(_current); }

	Renderer_DX11(CreateDesc& desc);

	DX11_IDXGIFactory*		dxgiFactory()		{ return _dxgiFactory; }
	DX11_IDXGIDevice*		dxgiDevice()		{ return _dxgiDevice; }
	DX11_IDXGIAdapter*		dxgiAdapter()		{ return _dxgiAdapter; }

	DX11_ID3DDevice*		d3dDevice()			{ return _d3dDevice; }
	DX11_ID3DDeviceContext* d3dDeviceContext()	{ return _d3dDeviceContext; }
	DX11_ID3DDebug*			d3dDebug()			{ return _d3dDebug; }

protected:
	virtual RenderContext*	onCreateContext(RenderContext_CreateDesc& desc);
	
	ComPtr<DX11_IDXGIFactory>		_dxgiFactory;
	ComPtr<DX11_IDXGIDevice>		_dxgiDevice;
	ComPtr<DX11_IDXGIAdapter>		_dxgiAdapter;

	ComPtr<DX11_ID3DDevice>			_d3dDevice;
	ComPtr<DX11_ID3DDeviceContext>	_d3dDeviceContext;
	ComPtr<DX11_ID3DDebug>			_d3dDebug;
};

}

#endif
