#include <sge_render/shader/Shader.h>

namespace sge {

class Shader_DX11 : public Shader {
	using Base = Shader;
public:
	Shader_DX11(StrView filename);

	class Pass : public ShaderPass {
	public:
		
	};
};

}