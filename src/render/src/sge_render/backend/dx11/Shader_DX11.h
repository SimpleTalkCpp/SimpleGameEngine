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

	protected:
		void _load(Pass* pass, StrView passPath);
		Pass* _pass = nullptr;
		Vector<u8> _bytecode;
		ShaderStageInfo _info;
	};
	
	struct VertexStage : public Stage<ShaderStageMask::Vertex> {
		void load(Pass* pass, StrView passPath, DX11_ID3DDevice* dev);
		ComPtr<DX11_ID3DVertexShader>	_shader;
	};

	struct PixelStage : public Stage<ShaderStageMask::Pixel> {
		void load(Pass* pass, StrView passPath, DX11_ID3DDevice* dev);
		ComPtr<DX11_ID3DPixelShader>	_shader;
	};

	struct Pass : public ShaderPass {
		Pass(Shader_DX11* shader, StrView passPath, ShaderInfo::Pass& info);

		Shader_DX11*		_shader = nullptr;
		ShaderInfo::Pass*	_info = nullptr;
		VertexStage			_vertexStage;
		PixelStage			_pixelStage;
	};

	Vector_<UPtr<Pass>, 1>	_passes;
};

}