#pragma once

#include "ShaderInfo.h"

namespace sge {

struct ShaderStage : public NonCopyable {
	const ShaderStageInfo* info() const { return &_info; }

protected:
	ShaderStageInfo _info;
};

struct ShaderPass : public NonCopyable {

};

class Shader : public RefCountBase {
public:
	Shader(StrView filename);
	virtual ~Shader();

	const String& filename() const { return _filename; }

protected:
	String	_filename;
	ShaderInfo	_info;
};

}