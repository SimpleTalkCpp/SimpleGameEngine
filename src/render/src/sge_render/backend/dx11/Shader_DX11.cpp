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
		auto* pass = new MyPass(this, passPath, _info.passes[i]);
		_passes.emplace_back(pass);
	}
}

void Shader_DX11::_loadStageFile(StrView passPath, ShaderStageMask stageMask, Vector<u8>& outBytecode, ShaderStageInfo& outInfo) {
	auto* profile = Util::getDxStageProfile(stageMask);

	auto filename = Fmt("{}/{}.bin", passPath, profile);
	File::readFile(filename, outBytecode);

	filename += ".json";
	JsonUtil::readFile(filename, outInfo);
}

void Shader_DX11::MyVertexStage::load(MyPass* pass, StrView passPath, DX11_ID3DDevice* dev) {
	_loadStageFile(passPath, stageMask(), _bytecode, _info);
	auto hr = dev->CreateVertexShader(_bytecode.data(), _bytecode.size(), nullptr, _shader.ptrForInit());
	Util::throwIfError(hr);
}

void Shader_DX11::MyPixelStage::load(MyPass* pass, StrView passPath, DX11_ID3DDevice* dev) {
	_loadStageFile(passPath, stageMask(), _bytecode, _info);
	auto hr = dev->CreatePixelShader(_bytecode.data(), _bytecode.size(), nullptr, _shader.ptrForInit());
	Util::throwIfError(hr);
}

void Shader_DX11::MyVertexStage::bind(RenderContext_DX11* ctx) {
	auto* dc = ctx->renderer()->d3dDeviceContext();
	if (!_shader) throw SGE_ERROR("dx shader is null");
	dc->VSSetShader(_shader, 0, 0);
}

void Shader_DX11::MyPixelStage::bind(RenderContext_DX11* ctx) {
	auto* dc = ctx->renderer()->d3dDeviceContext();
	if (!_shader) throw SGE_ERROR("dx shader is null");
	dc->PSSetShader(_shader, 0, 0);
}

Shader_DX11::MyPass::MyPass(Shader_DX11* shader, StrView passPath, ShaderInfo::Pass& info) 
	: Base(shader, info)
{
	_vertexStage = &_myVertexStage;
	_pixelStage  = &_myPixelStage;

	auto* renderer = Renderer_DX11::instance();
	auto* dev = renderer->d3dDevice();

	if (info.vsFunc.size()) { _myVertexStage.load(this, passPath, dev); }
	if (info.psFunc.size()) {  _myPixelStage.load(this, passPath, dev); }
}

}

