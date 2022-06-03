#include "Shader_DX11.h"
#include "Renderer_DX11.h"
#include "RenderContext_DX11.h"

namespace sge {

Shader_DX11::Shader_DX11(StrView filename) 
	: Base(filename)
{
	auto* proj = ProjectSettings::instance();
	TempString passPath;

	size_t n = _info.passes.size();
	_passes.reserve(n);
	for (size_t i = 0; i < n; i++) {
		FmtTo(passPath, "{}/{}/dx11/pass{}", proj->importedPath(), filename, i);
		auto* pass = new Pass(this, passPath, _info.passes[i]);
		_passes.emplace_back(pass);
	}
}

template<ShaderStageMask STAGE_MASK>
void Shader_DX11::Stage<STAGE_MASK>::_load(Pass* pass, StrView passPath) {
	_pass = pass;
	auto* profile = Util::getDxStageProfile(stageMask());

	auto filename = Fmt("{}/{}.bin", passPath, profile);
	File::readFile(filename, _bytecode);

	filename += ".json";
	JsonUtil::readFile(filename, _info);
}

void Shader_DX11::VertexStage::load(Pass* pass, StrView passPath, DX11_ID3DDevice* dev) {
	_load(pass, passPath);
	auto hr = dev->CreateVertexShader(_bytecode.data(), _bytecode.size(), nullptr, _shader.ptrForInit());
	Util::throwIfError(hr);
}

void Shader_DX11::PixelStage::load(Pass* pass, StrView passPath, DX11_ID3DDevice* dev) {
	_load(pass, passPath);
	auto hr = dev->CreatePixelShader(_bytecode.data(), _bytecode.size(), nullptr, _shader.ptrForInit());
	Util::throwIfError(hr);
}

void Shader_DX11::VertexStage::bind(RenderContext_DX11* ctx) {
	auto* dc = ctx->renderer()->d3dDeviceContext();
	if (!_shader) throw SGE_ERROR("dx shader is null");
	dc->VSSetShader(_shader, 0, 0);
}

void Shader_DX11::PixelStage::bind(RenderContext_DX11* ctx) {
	auto* dc = ctx->renderer()->d3dDeviceContext();
	if (!_shader) throw SGE_ERROR("dx shader is null");
	dc->PSSetShader(_shader, 0, 0);
}

Shader_DX11::Pass::Pass(Shader_DX11* shader, StrView passPath, ShaderInfo::Pass& info) 	
{
	_shader = shader;
	_info = &info;

	auto* renderer = Renderer_DX11::instance();
	auto* dev = renderer->d3dDevice();

	if (info.vsFunc.size()) { _vertexStage.load(this, passPath, dev); }
	if (info.psFunc.size()) {  _pixelStage.load(this, passPath, dev); }
}

}

