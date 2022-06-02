#include "Material.h"

namespace sge {

void Material::setShader(Shader* shader) {
	_shader = shader;
	onSetShader(shader);
}

}

