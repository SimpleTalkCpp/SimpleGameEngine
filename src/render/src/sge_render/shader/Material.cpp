#include "Material.h"

namespace sge {

void Material::setShader(Shader* shader) {
	_shader = shader;
	onSetShader(shader);
}

void Material::setParam(StrView name, float v) {
	SGE_ASSERT(false);
}

}

