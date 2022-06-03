#include <sge_render/shader/Material.h>
#include "Shader_DX11.h"
#include "RenderGpuBuffer_DX11.h"

namespace sge {

class Material_DX11 : public Material {
public:

private:
	using Util = DX11Util;
	Shader_DX11* shader() { return static_cast<Shader_DX11*>(_shader.ptr()); }

	struct MyVertexStage : public VertexStage {
		using Base = VertexStage;
		MyVertexStage(MaterialPass* pass, ShaderVertexStage* shaderStage) : Base(pass, shaderStage) {}

		void bind(RenderContext_DX11* ctx, const VertexLayout* vertexLayout);
		void bindInputLayout(RenderContext_DX11* ctx, const VertexLayout* vertexLayout);

		VectorMap<const VertexLayout*, ComPtr<DX11_ID3DInputLayout>> _inputLayoutsMap;
	};

	struct MyPixelStage : public PixelStage {
		using Base = PixelStage;

		MyPixelStage(MaterialPass* pass, ShaderPixelStage* shaderStage) : Base(pass, shaderStage) {}
		void bind(RenderContext_DX11* ctx, const VertexLayout* vertexLayout);
	};

	struct MyPass : public Pass {
		MyPass(Material* material, ShaderPass* shaderPass);

		virtual void onBind(RenderContext* ctx, const VertexLayout* vertexLayout) override;

		MyVertexStage _myVertexStage;
		MyPixelStage  _myPixelStage;
	};

	virtual Pass* onCreatePass(Material* material, ShaderPass* shaderPass) override {
		return new MyPass(material, shaderPass);
	}
};

}