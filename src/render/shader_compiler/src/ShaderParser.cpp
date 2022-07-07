#include "ShaderParser.h"

namespace sge {

void ShaderParser::readFile(ShaderInfo& outInfo, StrView filename) {
	_memMapFile.open(filename);
	readMem(outInfo, _memMapFile, filename);
}

void ShaderParser::readMem(ShaderInfo& outInfo, ByteSpan data, StrView filename) {
	outInfo.clear();
	_outInfo = &outInfo;
	reset(data, filename);
	skipNewlineTokens();

	if (_token.isIdentifier("Shader")) {
		_readShader();
	} else {
		error("missing Shader tag");
	}
}

void ShaderParser::_readShader() {
	nextToken();
	expectOperator("{");

	for(;;) {
		if (_token.isOperator("}"))		{ nextToken(); break; }
		if (_token.isNewline())			{ nextToken(); continue; }
		if (_token.isIdentifier("Properties"))	{ _readProperties(); continue; }
		if (_token.isIdentifier("Pass"))		{ _readPass(); continue; }
		return errorUnexpectedToken();
	}
}

void ShaderParser::_readProperties() {
	nextToken();
	expectOperator("{");

	for(;;) {
		skipNewlineTokens();
		if (_token.isOperator("}"))	{ nextToken(); break; }
		_readProperty();
	}	
}

void ShaderParser::_readProperty() {
	auto& o = _outInfo->props.emplace_back();

	// optional attribute
	if (_token.isOperator("[")) {
		nextToken();
		while (!_token.isNone()) {
			skipNewlineTokens();

			if (_token.isIdentifier("DisplayName")) {
				nextToken();
				expectOperator("=");
				readString(o.displayName);
			}
			if (_token.isOperator("]"))	{ nextToken(); break; }

			expectOperator(",");
		}
	}

	skipNewlineTokens();

	{
		// prop type
		readEnum(o.propType);

		// prop name
		readIdentifier(o.name);
	}

	// optional defaultValue
	if (_token.isOperator("=")) {
		nextToken();
		while (!_token.isNone()) {
			if (_token.isNewline())	{ break; }
			o.defaultValue += _token.str;
			nextToken();
		}
	}

	if (!_token.isNewline()) {
		errorUnexpectedToken();
	}
	nextToken();
}

void ShaderParser::_readPass() {
	nextToken();
	auto& o = _outInfo->passes.emplace_back();

	if (_token.isString()) {
		readString(o.name);
	}
	expectOperator("{");

	for(;;) {
		if (_token.isOperator("}"))	{ nextToken(); break; }
		if (_token.isNewline())		{ nextToken(); continue; }

		if (_token.isIdentifier("VsFunc")) { nextToken(); readIdentifier(o.vsFunc); continue; }
		if (_token.isIdentifier("PsFunc")) { nextToken(); readIdentifier(o.psFunc); continue; }

		if (_token.isIdentifier("Cull")) { nextToken(); readEnum(o.renderState.cull); continue; }

		if (_token.isIdentifier("DepthTest") ) { nextToken(); readEnum(o.renderState.depthTest.op); continue; }
		if (_token.isIdentifier("DepthWrite")) { nextToken(); readBool(o.renderState.depthTest.writeMask); continue; }

		if (_token.isIdentifier("BlendRGB")   ) { nextToken(); _readBlendFunc(o.renderState.blend.rgb); continue; }
		if (_token.isIdentifier("BlendAlpha") ) { nextToken(); _readBlendFunc(o.renderState.blend.rgb); continue; }

		return errorUnexpectedToken();
	}
}

void ShaderParser::_readBlendFunc(RenderState::BlendFunc& v) {
	readEnum(v.op);
	readEnum(v.srcFactor);
	readEnum(v.dstFactor);
}

}