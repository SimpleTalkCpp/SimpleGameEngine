#pragma once

#include "EditMesh.h"

namespace sge {

class WavefrontObjLoader : public NonCopyable {
public:
	static void loadFile	(EditMesh& mesh, StrView filename);
	static void loadMem		(EditMesh& mesh, Span<const u8> src);

private:
	void _loadMem	(EditMesh& mesh, Span<const u8> src);

	void _nextLine();
	void _nextToken();
	void _parseLine();
	void _parseLine_v();
	void _parseLine_vt();
	void _parseLine_vn();
	void _parseLine_f();

	void _error(StrView msg) { throw SGE_ERROR("{}: {}", _lineNumber, msg); }

	EditMesh*	_outMesh = nullptr;

	StrView	_source;
	StrView	_token;
	StrView	_currentLine;
	StrView	_lineRemain;
	StrView	_sourceRemain;

	int _lineNumber = 0;

	Vector<Tuple3f> _tmpPos;
	Vector<Tuple2f> _tmpUv;
	Vector<Tuple3f> _tmpNormal;
};

}