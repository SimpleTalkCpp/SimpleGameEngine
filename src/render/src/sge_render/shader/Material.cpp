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
{
	auto* renderer = Renderer::instance();

	auto* info = shaderStage->info();
	auto cbCount = info->constBuffers.size();
	_constBuffers.resize(cbCount);

	for (size_t i = 0; i < cbCount; i++) {
		auto& cb = _constBuffers[i];
		auto& cbInfo = info->constBuffers[i];
		
		cb.cpuBuffer.resize(cbInfo.dataSize);

		RenderGpuBuffer::CreateDesc desc;
		desc.type = RenderGpuBufferType::Const;
		desc.bufferSize = cbInfo.dataSize;

		cb.gpuBuffer = renderer->createGpuBuffer(desc);
	}
}

}

