#pragma once

#include "Shader.h"
#include <sge_render/buffer/RenderGpuBuffer.h>

namespace sge {

class RenderContext;
class Material;
class MaterialPass;

struct MaterialPass_Stage : public NonCopyable {
	virtual ~MaterialPass_Stage() = default;

	MaterialPass_Stage(MaterialPass* pass, ShaderStage* shaderStage);

protected:
	struct ConstBuffer {
		Vector<u8>				cpuBuffer;
		SPtr<RenderGpuBuffer>	gpuBuffer;
	};

	using Pass = MaterialPass;

	Pass*	_pass = nullptr;
	Vector_<ConstBuffer, 4>	_constBuffers;
};

struct MaterialPass_VertexStage : public MaterialPass_Stage {
	using Base = MaterialPass_Stage;

	MaterialPass_VertexStage(MaterialPass* pass, ShaderVertexStage* shaderStage) 
		: Base(pass, shaderStage)
		, _shaderStage(shaderStage)
	{}

	const ShaderStageInfo* info() const { return _shaderStage->info(); }
protected:
	ShaderVertexStage*	_shaderStage = nullptr;
};

struct MaterialPass_PixelStage  : public MaterialPass_Stage {
	using Base = MaterialPass_Stage;
	MaterialPass_PixelStage(MaterialPass* pass, ShaderPixelStage* shaderStage) 
		: Base(pass, shaderStage)
		, _shaderStage(shaderStage)
	{}

	const ShaderStageInfo* info() const { return _shaderStage->info(); }
protected:
	ShaderPixelStage*	_shaderStage = nullptr;
};

class MaterialPass : public RefCountBase {
public:
	virtual ~MaterialPass() = default;

	using Pass			= MaterialPass;
	using Stage			= MaterialPass_Stage;
	using VertexStage	= MaterialPass_VertexStage;
	using PixelStage	= MaterialPass_PixelStage;

	void bind(RenderContext* ctx, const VertexLayout* vertexLayout) { onBind(ctx, vertexLayout); }

friend class Material;
protected:

	MaterialPass(Material* material, ShaderPass* shaderPass) 
		: _material(material)
		, _shaderPass(shaderPass)
	{}

	Material*	_material	= nullptr;
	ShaderPass*	_shaderPass	= nullptr;

	virtual void onBind(RenderContext* ctx, const VertexLayout* vertexLayout) = 0;

	VertexStage*	_vertexStage = nullptr;
	PixelStage*		_pixelStage  = nullptr;
};

class Material : public RefCountBase {
public:
	virtual ~Material() = default;

	void setShader(Shader* shader);

//	void setParam(StrView name, const Tuple4f& v) { onSetParam(name, v); }
//	void setParam(StrView name, const Color4f& v) { setParam(name, v.toTuple); }

	using Pass			= MaterialPass;
	using Stage			= MaterialPass_Stage;
	using VertexStage	= MaterialPass_VertexStage;
	using PixelStage	= MaterialPass_PixelStage;

	Span<SPtr<Pass>>	passes() { return _passes; }

protected:
	Vector_<SPtr<Pass>, 1>	_passes;
	SPtr<Shader> _shader;
	virtual void onSetShader() {}
	virtual Pass* onCreatePass(Material* material, ShaderPass* shaderPass) = 0;
};

}
