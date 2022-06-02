#pragma once

#include "ShaderInfo.h"

namespace sge {

class ShaderPass {
public:
};

class Shader : public RefCountBase {
public:
	Shader(StrView filename);
	virtual ~Shader();

	const String& filename() const { return _filename; }

private:
	String	_filename;
};

}