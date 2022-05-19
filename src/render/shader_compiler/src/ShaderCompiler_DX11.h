#pragma once

#include <sge_render.h>

namespace sge {

class ShaderCompiler_DX11 : public NonCopyable {
public:
	void compile(RenderShaderType shaderType, StrView filename);
};

}