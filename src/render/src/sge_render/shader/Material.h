#pragma once

#include "Shader.h"

namespace sge {

class Material : public RefCountBase {
public:
	virtual ~Material() = default;

	void setShader(Shader* shader);

	void setParam(StrView name, float v);

private:
	SPtr<Shader> _shader;
	virtual void onSetShader(Shader* shader) = 0;
};

}
