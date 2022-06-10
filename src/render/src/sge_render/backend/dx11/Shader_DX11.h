#pragma once

#include <sge_render/shader/Shader.h>
#include <sge_render/backend/dx11/Render_DX11_Common.h>

namespace sge {

class Shader_DX11 : public Shader {
	using Base = Shader;
public:
	using Util = DX11Util;

	Shader_DX11(StrView filename);

	struct MyPass;

	static void _loadStageFile(StrView passPath, ShaderStageMask stageMask, Vector<u8>& outBytecode, ShaderStageInfo& outInfo);

	struct MyVertexStage : public ShaderVertexStage {
		void load(MyPass* pass, StrView passPath, DX11_ID3DDevice* dev);
		void bind(RenderContext_DX11* ctx);
		ByteSpan bytecode() const { return _bytecode; }
	private:
		ComPtr<DX11_ID3DVertexShader>	_shader;
		Vector<u8> _bytecode;
	};

	struct MyPixelStage : public ShaderPixelStage {
		void load(MyPass* pass, StrView passPath, DX11_ID3DDevice* dev);
		void bind(RenderContext_DX11* ctx);
		ByteSpan bytecode() const { return _bytecode; }
	private:
		ComPtr<DX11_ID3DPixelShader>	_shader;
		Vector<u8> _bytecode;
	};

	struct MyPass : public ShaderPass {
		using Base = ShaderPass;

		MyPass(Shader_DX11* shader, StrView passPath, ShaderInfo::Pass& info);
	private:
		MyVertexStage		_myVertexStage;
		MyPixelStage		_myPixelStage;
	};
};

}