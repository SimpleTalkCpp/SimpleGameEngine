#include "Material.h"

namespace sge {

void Material::setShader(Shader* shader) {
	if (_shader == shader) return;

	_shader = shader;
	onSetShader();
}

void Material::setParam(StrView name, float v) {
}

}

