#include "Shader.h"
#include <sge_render/Renderer.h>

namespace sge {

Shader::Shader(StrView filename) 
	: _filename(filename)
{
}

Shader::~Shader() {
	auto* renderer = Renderer::instance();
	renderer->onShaderDestory(this);
}

}

