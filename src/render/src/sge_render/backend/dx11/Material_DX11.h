#include <sge_render/shader/Material.h>
#include "Shader_DX11.h"
#include "RenderGpuBuffer_DX11.h"

namespace sge {

class Material_DX11 : public Material {
public:	
	void bind(RenderContext_DX11* ctx, const VertexLayout* vertexLayout);

private:
	using Util = DX11Util;

	Shader_DX11*	shader() { return static_cast<Shader_DX11*>(_shader.ptr()); }
	virtual void onSetShader() override;

	struct Pass;

	template<class SHADER_STAGE>
	struct Stage : public NonCopyable {
		void reset(Pass* pass, SHADER_STAGE* _shaderStage);
	protected:
		Pass* _pass = nullptr;
		SHADER_STAGE* _shaderStage = nullptr;
		Vector_<SPtr<RenderGpuBuffer_DX11>, 4>	_constBuffers;
	};

	struct VertexStage : public Stage<Shader_DX11::VertexStage> {
		void bind(RenderContext_DX11* ctx, const VertexLayout* vertexLayout);
		void bindInputLayout(RenderContext_DX11* ctx, const VertexLayout* vertexLayout);

		VectorMap<const VertexLayout*, ComPtr<DX11_ID3DInputLayout>> _inputLayoutsMap;
	};
	struct PixelStage : public Stage<Shader_DX11::PixelStage> {
		void bind(RenderContext_DX11* ctx, const VertexLayout* vertexLayout);
	};

	struct Pass : public NonCopyable {
		Pass(Material_DX11* material, Shader_DX11::Pass* shPass);

		void bind(RenderContext_DX11* ctx, const VertexLayout* vertexLayout);

		Material_DX11*				_material	= nullptr;
		const Shader_DX11::Pass*	_shPass		= nullptr;

		VertexStage _vertexStage;
		PixelStage  _pixelStage;
	};

	Vector_<UPtr<Pass>, 1>	_passes;
};

}