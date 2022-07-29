#pragma once

#if SGE_RENDER_HAS_DX11

#include <d3d11.h>
#include <d3d11_4.h>
#include <dxgi1_4.h>

#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <sge_render/Render_Common.h>
#include <sge_render/vertex/Vertex.h>
#include <sge_render/shader/Shader.h>
#include <sge_render/textures/Texture.h>

namespace sge {

class Renderer_DX11;
class RenderContext_DX11;

using DX11_IDXGIFactory				= IDXGIFactory1;
using DX11_IDXGIDevice				= IDXGIDevice;
using DX11_IDXGIAdapter				= IDXGIAdapter3;
using DX11_IDXGISwapChain			= IDXGISwapChain;

using DX11_ID3DDevice				= ID3D11Device1;
using DX11_ID3DDeviceContext		= ID3D11DeviceContext4;
using DX11_ID3DDebug				= ID3D11Debug;

using DX11_ID3DTexture1D			= ID3D11Texture1D;
using DX11_ID3DTexture2D			= ID3D11Texture2D;
using DX11_ID3DTexture3D			= ID3D11Texture3D;

using DX11_ID3DSamplerState			= ID3D11SamplerState;
using DX11_ID3DShaderResourceView	= ID3D11ShaderResourceView;

using DX11_ID3DRenderTargetView		= ID3D11RenderTargetView;
using DX11_ID3DDepthStencilView		= ID3D11DepthStencilView;

using DX11_ID3DBuffer				= ID3D11Buffer;
using DX11_ID3DBlob					= ID3DBlob;

using DX11_ID3DVertexShader			= ID3D11VertexShader;
using DX11_ID3DPixelShader			= ID3D11PixelShader;
using DX11_ID3DInputLayout			= ID3D11InputLayout;

using DX11_ID3DRasterizerState		= ID3D11RasterizerState;
using DX11_ID3DDepthStencilState	= ID3D11DepthStencilState;
using DX11_ID3DBlendState			= ID3D11BlendState;

struct DX11Util {
	DX11Util() = delete;

	static void throwIfError	(HRESULT hr) { 
		if (!_checkError(hr)) { reportError(hr); throw SGE_ERROR("HRESULT = {}", hr); }
	}
	static bool assertIfError	(HRESULT hr) {
		if (!_checkError(hr)) { reportError(hr); SGE_ASSERT(false); return false; }
		return true;
	}
	static void reportError(HRESULT hr);

	static UINT castUINT(size_t v) { SGE_ASSERT(v < UINT_MAX); return static_cast<UINT>(v); }

	static D3D11_PRIMITIVE_TOPOLOGY	getDxPrimitiveTopology	(RenderPrimitiveType t);
	static DXGI_FORMAT				getDxFormat				(RenderDataType v);
	static DXGI_FORMAT				getDxColorType			(ColorType v);

	static D3D11_CULL_MODE			getDxCullMode			(RenderState_Cull v);
	static D3D11_COMPARISON_FUNC	getDxDepthTestOp		(RenderState_DepthTestOp v);
	static D3D11_BLEND_OP			getDxBlendOp			(RenderState_BlendOp v);
	static D3D11_BLEND				getDxBlendFactor		(RenderState_BlendFactor v);

	static D3D11_FILTER					getDxTextureFilter	(TextureFilter v);
	static D3D11_TEXTURE_ADDRESS_MODE	getDxTextureWrap	(TextureWrap v);

	static const char* getDxSemanticName(VertexSemanticType t);
	static VertexSemanticType		parseDxSemanticName		(StrView s);

	static String getStrFromHRESULT(HRESULT hr);

	static const char* getDxStageProfile(ShaderStageMask s);

	static ByteSpan toSpan(ID3DBlob* blob);
	static StrView  toStrView(ID3DBlob* blob) { return StrView_make(toSpan(blob)); }

private:
	static bool _checkError(HRESULT hr) {
		return SUCCEEDED(hr);
	}
};

inline
ByteSpan DX11Util::toSpan(ID3DBlob* blob) {
	if (!blob) return ByteSpan();
	return ByteSpan(reinterpret_cast<const u8*>(blob->GetBufferPointer()),
					static_cast<size_t>(blob->GetBufferSize()));
}

inline
String DX11Util::getStrFromHRESULT(HRESULT hr) {
	const int bufSize = 4096;
	wchar_t buf[bufSize + 1];

	DWORD langId = 0; // MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, hr, langId, buf, bufSize, nullptr );
	buf[bufSize] = 0; // ensure terminate with 0

	auto str = UtfUtil::toString(buf);
	return str;
}

inline
const char* DX11Util::getDxStageProfile(ShaderStageMask s) {
	switch (s) {
		case ShaderStageMask::Vertex:	return "vs_5_0";
		case ShaderStageMask::Pixel:	return "ps_5_0";
		default: return "";
	}
}

inline
void DX11Util::reportError(HRESULT hr) {
	if (!SUCCEEDED(hr)) {
		auto str = getStrFromHRESULT(hr);
		SGE_LOG("HRESULT(0x{:0X}) {}", static_cast<u32>(hr), str);
	}

#if 0 && _DEBUG
	auto* d = renderer()->d3dDebug();
	if (d) {
		d->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}
#endif
}

inline
D3D11_PRIMITIVE_TOPOLOGY DX11Util::getDxPrimitiveTopology(RenderPrimitiveType t) {
	using SRC = RenderPrimitiveType;
	switch (t) {
		case SRC::Points:		return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
		case SRC::Lines:		return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
		case SRC::Triangles:	return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		default: throw SGE_ERROR("unknown RenderPrimitiveType");
	}
}

inline
DXGI_FORMAT DX11Util::getDxFormat(RenderDataType v) {
	using SRC = RenderDataType;
	switch (v) {
		case SRC::Int8:			return DXGI_FORMAT_R8_SINT; break;
		case SRC::Int8x2:		return DXGI_FORMAT_R8G8_SINT; break;
//		case SRC::Int8x3:		return DXGI_FORMAT_R8G8B8_SINT; break; //does not support in DX11
		case SRC::Int8x4:		return DXGI_FORMAT_R8G8B8A8_SINT; break;

		case SRC::UInt8:		return DXGI_FORMAT_R8_UINT; break;
		case SRC::UInt8x2:		return DXGI_FORMAT_R8G8_UINT; break;
//		case SRC::UInt8x3:		return DXGI_FORMAT_R8G8B8_UINT; break; //does not support in DX11
		case SRC::UInt8x4:		return DXGI_FORMAT_R8G8B8A8_UINT; break;

		case SRC::SNorm8:		return DXGI_FORMAT_R8_SNORM; break;
		case SRC::SNorm8x2:		return DXGI_FORMAT_R8G8_SNORM; break;
//		case SRC::SNorm8x3:		return DXGI_FORMAT_R8G8B8_SNORM; break; //does not support in DX11
		case SRC::SNorm8x4:		return DXGI_FORMAT_R8G8B8A8_SNORM; break;

		case SRC::UNorm8:		return DXGI_FORMAT_R8_UNORM; break;
		case SRC::UNorm8x2:		return DXGI_FORMAT_R8G8_UNORM; break;
//		case SRC::UNorm8x3:		return DXGI_FORMAT_R8G8B8_UNORM; break; //does not support in DX11
		case SRC::UNorm8x4:		return DXGI_FORMAT_R8G8B8A8_UNORM; break;

		case SRC::Int16:		return DXGI_FORMAT_R16_SINT; break;
		case SRC::Int16x2:		return DXGI_FORMAT_R16G16_SINT; break;
//		case SRC::Int16x3:		return DXGI_FORMAT_R16G16B16_SINT; break; //does not support in DX11
		case SRC::Int16x4:		return DXGI_FORMAT_R16G16B16A16_SINT; break;

		case SRC::UInt16:		return DXGI_FORMAT_R16_UINT; break;
		case SRC::UInt16x2:		return DXGI_FORMAT_R16G16_UINT; break;
//		case SRC::UInt16x3:		return DXGI_FORMAT_R16G16B16_UINT; break; //does not support in DX11
		case SRC::UInt16x4:		return DXGI_FORMAT_R16G16B16A16_UINT; break;

		case SRC::SNorm16:		return DXGI_FORMAT_R16_SNORM; break;
		case SRC::SNorm16x2:	return DXGI_FORMAT_R16G16_SNORM; break;
//		case SRC::SNorm16x3:	return DXGI_FORMAT_R16G16B16_SNORM; break; //does not support in DX11
		case SRC::SNorm16x4:	return DXGI_FORMAT_R16G16B16A16_SNORM; break;

		case SRC::UNorm16:		return DXGI_FORMAT_R16_UNORM; break;
		case SRC::UNorm16x2:	return DXGI_FORMAT_R16G16_UNORM; break;
//		case SRC::UNorm16x3:	return DXGI_FORMAT_R16G16B16_UNORM; break; //does not support in DX11
		case SRC::UNorm16x4:	return DXGI_FORMAT_R16G16B16A16_UNORM; break;

		case SRC::Int32:		return DXGI_FORMAT_R32_SINT; break;
		case SRC::Int32x2:		return DXGI_FORMAT_R32G32_SINT; break;
//		case SRC::Int32x3:		return DXGI_FORMAT_R32G32B32_SINT; break; //does not support in DX11
		case SRC::Int32x4:		return DXGI_FORMAT_R32G32B32A32_SINT; break;

		case SRC::UInt32:		return DXGI_FORMAT_R32_UINT; break;
		case SRC::UInt32x2:		return DXGI_FORMAT_R32G32_UINT; break;
//		case SRC::UInt32x3:		return DXGI_FORMAT_R32G32B32_UINT; break; //does not support in DX11
		case SRC::UInt32x4:		return DXGI_FORMAT_R32G32B32A32_UINT; break;

//		case SRC::SNorm32:		return DXGI_FORMAT_R32_SNORM; break;
//		case SRC::SNorm32x2:	return DXGI_FORMAT_R32G32_SNORM; break;
//		case SRC::SNorm32x3:	return DXGI_FORMAT_R32G32B32_SNORM; break; //does not support in DX11
//		case SRC::SNorm32x4:	return DXGI_FORMAT_R32G32B32A32_SNORM; break;

//		case SRC::UNorm32:		return DXGI_FORMAT_R32_UNORM; break;
//		case SRC::UNorm32x2:	return DXGI_FORMAT_R32G32_UNORM; break;
//		case SRC::UNorm32x3:	return DXGI_FORMAT_R32G32B32_UNORM; break; //does not support in DX11
//		case SRC::UNorm32x4:	return DXGI_FORMAT_R32G32B32A32_UNORM; break;

	//--
		case SRC::Float16:		return DXGI_FORMAT_R16_FLOAT; break;
		case SRC::Float16x2:	return DXGI_FORMAT_R16G16_FLOAT; break;
//		case SRC::Float16x3:	return DXGI_FORMAT_R16G16B16_FLOAT; break; //does not support in DX11
		case SRC::Float16x4:	return DXGI_FORMAT_R16G16B16A16_FLOAT; break;
	//---
		case SRC::Float32:		return DXGI_FORMAT_R32_FLOAT; break;
		case SRC::Float32x2:	return DXGI_FORMAT_R32G32_FLOAT; break;
		case SRC::Float32x3:	return DXGI_FORMAT_R32G32B32_FLOAT; break;
		case SRC::Float32x4:	return DXGI_FORMAT_R32G32B32A32_FLOAT; break;
	//---
		default: throw SGE_ERROR("unsupported RenderDataType");
	}
}

inline
DXGI_FORMAT DX11Util::getDxColorType(ColorType v) {
	using SRC = ColorType;
	switch (v) {
		case SRC::Lb:		return DXGI_FORMAT_R8_UNORM;
		case SRC::Ls:		return DXGI_FORMAT_R16_UNORM;
		case SRC::Lf:		return DXGI_FORMAT_R32_FLOAT;

		case SRC::Rb:		return DXGI_FORMAT_R8_UNORM;
		case SRC::Rs:		return DXGI_FORMAT_R16_UNORM;
		case SRC::Rf:		return DXGI_FORMAT_R32_FLOAT;

		case SRC::RGb:		return DXGI_FORMAT_R8G8_UNORM;
		case SRC::RGs:		return DXGI_FORMAT_R16G16_UNORM;
		case SRC::RGf:		return DXGI_FORMAT_R32G32_FLOAT;

		//case SRC::RGBb:	return DXGI_FORMAT_R8G8B8_UNORM;		// DX Not Support
		//case SRC::RGBf:	return DXGI_FORMAT_R32G32B32_FLOAT;		// DX Not Support

		case SRC::RGBAb:	return DXGI_FORMAT_R8G8B8A8_UNORM;
		case SRC::RGBAs:	return DXGI_FORMAT_R16G16B16A16_UNORM;
		case SRC::RGBAf:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
//
		case SRC::BC1:		return DXGI_FORMAT_BC1_UNORM;
		case SRC::BC2:		return DXGI_FORMAT_BC2_UNORM;
		case SRC::BC3:		return DXGI_FORMAT_BC3_UNORM;
		case SRC::BC4:		return DXGI_FORMAT_BC4_UNORM;
		case SRC::BC5:		return DXGI_FORMAT_BC5_UNORM;
		case SRC::BC6h:		return DXGI_FORMAT_BC6H_UF16;
		case SRC::BC7:		return DXGI_FORMAT_BC7_UNORM;

		default: throw SGE_ERROR("unsupported ColorType");
	}
}


inline
D3D11_CULL_MODE DX11Util::getDxCullMode(RenderState_Cull v) {
	using SRC = RenderState_Cull;
	switch (v) {
		case SRC::None:		return D3D11_CULL_NONE;
		case SRC::Back:		return D3D11_CULL_BACK;
		case SRC::Front:	return D3D11_CULL_FRONT;
		default: throw SGE_ERROR("unsupported CullMode");
	}
}

inline
D3D11_COMPARISON_FUNC DX11Util::getDxDepthTestOp(RenderState_DepthTestOp v) {
	using SRC = RenderState_DepthTestOp;
	switch (v) {
		case SRC::Always:		return  D3D11_COMPARISON_ALWAYS;
		case SRC::Less:			return  D3D11_COMPARISON_LESS;
		case SRC::Equal:		return  D3D11_COMPARISON_EQUAL;
		case SRC::LessEqual:	return  D3D11_COMPARISON_LESS_EQUAL;
		case SRC::Greater:		return  D3D11_COMPARISON_GREATER;
		case SRC::GreaterEqual:	return  D3D11_COMPARISON_GREATER_EQUAL;
		case SRC::NotEqual:		return  D3D11_COMPARISON_NOT_EQUAL;
		case SRC::Never:		return  D3D11_COMPARISON_NEVER;
		default: throw SGE_ERROR("unsupported DepthTestOp");
	}
}

inline
D3D11_BLEND_OP DX11Util::getDxBlendOp(RenderState_BlendOp v) {
	using SRC = RenderState_BlendOp;
	switch (v) {
		case SRC::Add:		return D3D11_BLEND_OP_ADD;
		case SRC::Min:		return D3D11_BLEND_OP_MIN;
		case SRC::Max:		return D3D11_BLEND_OP_MAX;
		case SRC::Sub:		return D3D11_BLEND_OP_SUBTRACT;
		case SRC::RevSub:	return D3D11_BLEND_OP_REV_SUBTRACT;
		default: throw SGE_ERROR("unsupported BlendOp");
	}
}

inline
D3D11_BLEND DX11Util::getDxBlendFactor(RenderState_BlendFactor v) {
	using SRC = RenderState_BlendFactor;
	switch (v) {
		case SRC::Zero:					return D3D11_BLEND_ZERO;
		case SRC::One:					return D3D11_BLEND_ONE;
		case SRC::SrcAlpha:				return D3D11_BLEND_SRC_ALPHA;
		case SRC::DstAlpha:				return D3D11_BLEND_DEST_ALPHA;
		case SRC::SrcColor:				return D3D11_BLEND_SRC_COLOR;
		case SRC::DstColor:				return D3D11_BLEND_DEST_COLOR;
		case SRC::ConstColor:			return D3D11_BLEND_BLEND_FACTOR;
//		case SRC::ConstAlpha:			return 
		case SRC::OneMinusSrcAlpha:		return D3D11_BLEND_INV_SRC_ALPHA;
		case SRC::OneMinusSrcColor:		return D3D11_BLEND_INV_SRC_COLOR;
		case SRC::OneMinusDstAlpha:		return D3D11_BLEND_INV_DEST_ALPHA;
		case SRC::OneMinusDstColor:		return D3D11_BLEND_INV_DEST_COLOR;
		case SRC::OneMinusConstColor:	return D3D11_BLEND_INV_BLEND_FACTOR;
//		case SRC::OneMinusConstAlpha:	return 
		case SRC::SrcAlphaSaturate:		return D3D11_BLEND_SRC_ALPHA_SAT;
		default: throw SGE_ERROR("unsupported BlendFactor");
	}
}

inline D3D11_FILTER DX11Util::getDxTextureFilter(TextureFilter v) {
	using SRC = TextureFilter;
	switch (v) {
		case SRC::Point:		return D3D11_FILTER_MIN_MAG_MIP_POINT;
		case SRC::Linear:		return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		case SRC::Bilinear:		return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		case SRC::Trilinear:	return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		case SRC::Anisotropic:	return D3D11_FILTER_ANISOTROPIC;
	//---
		default: throw SGE_ERROR("unsupported TextureFilter");
	}
}

inline D3D11_TEXTURE_ADDRESS_MODE DX11Util::getDxTextureWrap(TextureWrap v) {
	using SRC = TextureWrap;
	switch (v) {
		case SRC::Repeat:		return D3D11_TEXTURE_ADDRESS_WRAP;
		case SRC::Clamp:		return D3D11_TEXTURE_ADDRESS_CLAMP;
		case SRC::Mirror:		return D3D11_TEXTURE_ADDRESS_MIRROR;
		case SRC::MirrorOnce:	return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
	//---
		default: throw SGE_ERROR("unsupported TextureWrap");
	}
}

} // namespace

#endif