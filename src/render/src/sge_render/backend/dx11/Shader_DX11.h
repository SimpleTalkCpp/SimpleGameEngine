#include <sge_render/shader/Shader.h>
#include <sge_render/backend/dx11/Render_DX11_Common.h>

namespace sge {

class Shader_DX11 : public Shader {
	using Base = Shader;
public:
	using Util = DX11Util;

	Shader_DX11(StrView filename);

	struct Pass;

	template<ShaderStageMask STAGE_MASK>
	struct Stage : public ShaderStage {
	public:
		constexpr ShaderStageMask	stageMask() const { return STAGE_MASK; }
		ByteSpan	bytecode() const { return _bytecode; }

	protected:
		void _load(Pass* pass, StrView passPath);
		Pass* _pass = nullptr;
		Vector<u8> _bytecode;
	};

	struct VertexStage : public Stage<ShaderStageMask::Vertex> {
		void load(Pass* pass, StrView passPath, DX11_ID3DDevice* dev);
		void bind(RenderContext_DX11* ctx);
	private:
		ComPtr<DX11_ID3DVertexShader>	_shader;
	};

	struct PixelStage : public Stage<ShaderStageMask::Pixel> {
		void load(Pass* pass, StrView passPath, DX11_ID3DDevice* dev);
		void bind(RenderContext_DX11* ctx);
	private:
		ComPtr<DX11_ID3DPixelShader>	_shader;
	};

	struct Pass : public ShaderPass {
		Pass(Shader_DX11* shader, StrView passPath, ShaderInfo::Pass& info);

		const ShaderInfo::Pass*	info() const { return _info; }

		VertexStage*	vertexStage()	{ return &_vertexStage; }
		PixelStage*		pixelStage()	{ return &_pixelStage;  }

	private:
		Shader_DX11*		_shader = nullptr;
		ShaderInfo::Pass*	_info = nullptr;
		VertexStage			_vertexStage;
		PixelStage			_pixelStage;
	};

	size_t	passCount() const		{ return _passes.size(); }
	Pass*	getPass(size_t i) const { return _passes[i].get(); }

private:
	Vector_<UPtr<Pass>, 1>	_passes;
};

}