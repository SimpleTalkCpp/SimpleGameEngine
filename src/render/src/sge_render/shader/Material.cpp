#include "Material.h"
#include <sge_render/Renderer.h>

namespace sge {

void Material::setShader(Shader* shader) {
	if (_shader == shader) return;
	_shader = shader;
	_passes.clear();
	_passes.reserve(shader->passes().size());
	for (auto& shaderPass : shader->passes()) {
		auto* pass = onCreatePass(this, shaderPass.get());
		_passes.emplace_back(pass);
	}

	onSetShader();
}

MaterialPass_Stage::MaterialPass_Stage(MaterialPass* pass, ShaderStage* shaderStage) 
	: _pass(pass)
	, _shaderStage(shaderStage)
{
	auto* info = shaderStage->info();
	auto cbCount = info->constBuffers.size();
	_constBuffers.resize(cbCount);

	for (size_t i = 0; i < cbCount; i++) {
		auto& cb = _constBuffers[i];	
		cb.create(info->constBuffers[i]);
	}
}

void MaterialPass_Stage::ConstBuffer::create(const Info& info) {
	_info = &info;
	_gpuDirty = true;

	cpuBuffer.resize(info.dataSize);

	RenderGpuBuffer::CreateDesc desc;
	desc.type = RenderGpuBufferType::Const;
	desc.bufferSize = info.dataSize;

	gpuBuffer = Renderer::instance()->createGpuBuffer(desc);
}


void MaterialPass_Stage::ConstBuffer::uploadToGpu() {
	if (!_gpuDirty) return;
	_gpuDirty = false;

	if (!gpuBuffer) return;
	gpuBuffer->uploadToGpu(cpuBuffer);
}

void MaterialPass_Stage::ConstBuffer::errorType() {
	throw SGE_ERROR("ConstBuffer setParam type mismatch");
}

}

