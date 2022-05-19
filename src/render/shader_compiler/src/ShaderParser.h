#pragma once

#include <sge_core/string/Lexer.h>
#include <sge_render/shader/ShaderInfo.h>

namespace sge {

class ShaderParser : protected Lexer {
public:
	void readFile(ShaderInfo& outInfo, StrView filename);
	void readMem(ShaderInfo& outInfo, Span<const u8> data, StrView filename);

private:
	void _readShader();
	void _readProperties();
	void _readProperty();
	void _readPass();

	template<class E> void _readEnum(E& v);

	ShaderInfo* _outInfo = nullptr;
};

template<class E> inline
void ShaderParser::_readEnum(E& v) {
	if (!token().isIdentifier()) {
		errorUnexpectedToken();
		return;
	}

	if (!enumTryParse(v, _token.str)) {
		error("read enum [{}]", _token.str);
		return;
	}
	nextToken();
}

}