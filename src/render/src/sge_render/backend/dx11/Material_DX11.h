#include <sge_render/shader/Material.h>
#include "Shader_DX11.h"

namespace sge {

class Material_DX11 : public Material {
public:
	

private:
	virtual void onSetShader(Shader* shader) override;
};

}