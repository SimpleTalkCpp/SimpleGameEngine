#include "Renderer_DX11.h"
#include "RenderContext_DX11.h"
#include "RenderGpuBuffer_DX11.h"
#include "Material_DX11.h"
#include "Texture_DX11.h"

namespace sge {

Renderer_DX11::Renderer_DX11(CreateDesc& desc) {
	D3D_FEATURE_LEVEL featureLevel;
	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	ComPtr<ID3D11Device>			d3dDevice;
	ComPtr<ID3D11DeviceContext>		d3dDeviceContext;

	HRESULT hr;
	hr = D3D11CreateDevice(	nullptr,
							D3D_DRIVER_TYPE_HARDWARE,
							nullptr,
							createDeviceFlags,
							nullptr,
							0, 
							D3D11_SDK_VERSION,
							d3dDevice.ptrForInit(),
							&featureLevel,
							d3dDeviceContext.ptrForInit());
	Util::throwIfError(hr);
	//This method returns DXGI_ERROR_SDK_COMPONENT_MISSING if you specify D3D11_CREATE_DEVICE_DEBUG in Flags
	//and the incorrect version of the debug layer is installed on your computer. Install the latest Windows SDK to get the correct version.

	hr = d3dDevice->QueryInterface(IID_PPV_ARGS(_d3dDevice.ptrForInit()));
	Util::throwIfError(hr);

	hr = d3dDeviceContext->QueryInterface(IID_PPV_ARGS(_d3dDeviceContext.ptrForInit()));
	Util::throwIfError(hr);

	hr = _d3dDevice->QueryInterface(IID_PPV_ARGS(_dxgiDevice.ptrForInit()));
	Util::throwIfError(hr);

	{
		ComPtr<IDXGIAdapter> adapter;
		hr = _dxgiDevice->GetAdapter(adapter.ptrForInit());
		Util::throwIfError(hr);

		hr = adapter->QueryInterface(IID_PPV_ARGS(_dxgiAdapter.ptrForInit()));
		Util::throwIfError(hr);
	}

	{
		D3D11_FEATURE_DATA_THREADING s = {};
		hr = d3dDevice->CheckFeatureSupport(D3D11_FEATURE_THREADING, &s, sizeof(s));
		Util::throwIfError(hr);
		_adapterInfo.multithread = s.DriverConcurrentCreates && s.DriverCommandLists;
		if (!_adapterInfo.multithread)
			desc.multithread = false;
	}

	//{
	//	D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS s = {};
	//	hr = d3dDevice->CheckFeatureSupport(D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS, &s, sizeof(s));
	//	Util::throwIfError(hr);
	//	_adapterInfo.computeShader = s.ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x;
	//}
	_adapterInfo.hasComputeShader = true;

	{
		D3D11_FEATURE_DATA_DOUBLES s = {};
		hr = d3dDevice->CheckFeatureSupport(D3D11_FEATURE_DOUBLES, &s, sizeof(s));
		Util::throwIfError(hr);
		_adapterInfo.shaderHasFloat64 = s.DoublePrecisionFloatShaderOps;
	}

	{
		DXGI_ADAPTER_DESC ad;
		hr = _dxgiAdapter->GetDesc(&ad);
		Util::throwIfError(hr);

		_adapterInfo.adapterName = UtfUtil::toString(ad.Description);
		_adapterInfo.memorySize = ad.DedicatedVideoMemory;

		SGE_LOG("Render Adapter\n  name={}\n  mem={}G"
					, _adapterInfo.adapterName
					, Math::byteToG(_adapterInfo.memorySize));
	}

	hr = _dxgiAdapter->GetParent(IID_PPV_ARGS(_dxgiFactory.ptrForInit()));
	Util::throwIfError(hr);

	if (createDeviceFlags & D3D11_CREATE_DEVICE_DEBUG) {
		hr = _d3dDevice->QueryInterface(_d3dDebug.ptrForInit());
		Util::throwIfError(hr);
	}

}

SPtr<RenderContext> Renderer_DX11::onCreateContext(RenderContext_CreateDesc& desc) {
	return new RenderContext_DX11(desc);
}

SPtr<RenderGpuBuffer> Renderer_DX11::onCreateGpuBuffer(RenderGpuBuffer_CreateDesc& desc) {
	SPtr<RenderGpuBuffer> p = new RenderGpuBuffer_DX11();
	p->create(desc);
	return p;
}

SPtr<Texture2D> Renderer_DX11::onCreateTexture2D(Texture2D_CreateDesc& desc) {
	return new Texture2D_DX11(desc);
}

SPtr<Material> Renderer_DX11::onCreateMaterial() {
	return new Material_DX11();
}

SPtr<Shader> Renderer_DX11::onCreateShader(StrView filename) {
	return new Shader_DX11(filename);
}

void Renderer_DX11::validateContext() {
	if (!_d3dDebug) return;
	auto hr = _d3dDebug->ValidateContext(_d3dDeviceContext.ptr());
	Util::throwIfError(hr);
}


}
