#include "WavefrontObjLoader.h"

namespace sge {

void WavefrontObjLoader::loadFile(EditMesh& mesh, StrView filename) {
	MemMapFile mm;
	mm.open(filename);
	loadMem(mesh, mm);
}

void WavefrontObjLoader::loadMem(EditMesh& mesh, ByteSpan src) {
	WavefrontObjLoader inst;
	inst._loadMem(mesh, src);
}

void WavefrontObjLoader::_loadMem(EditMesh& mesh, ByteSpan src) {
	mesh.clear();
	_outMesh = &mesh;
	_outMesh->primitive = RenderPrimitiveType::Triangles;

	_source = StrView_make(src);
	_sourceRemain = _source;
	_lineNumber = 0;

	while (_sourceRemain.size() > 0) {
		_nextLine();
		if (_currentLine.size()) {
			_lineRemain = _currentLine;
			_nextToken();
			_parseLine();
		}
	}
}

void WavefrontObjLoader::_nextLine() {
	auto ret = StringUtil::splitByChar(_sourceRemain, "\n");
	_currentLine  = ret.first;
	_sourceRemain = ret.second;
	_lineNumber++;
}

void WavefrontObjLoader::_nextToken() {
	auto ret    = StringUtil::splitByChar(_lineRemain, " \t\r");
	_token      = StringUtil::trimChar(ret.first,  " \t\r");
	_lineRemain = StringUtil::trimChar(ret.second, " \t\r");
}

void WavefrontObjLoader::_parseLine() {
	if (_token.size() <= 0) return;
	if (_token[0] == '#') return; //skip comment

	if (_token == "v" ) return _parseLine_v();
	if (_token == "vt") return _parseLine_vt();
	if (_token == "vn") return _parseLine_vn();
	if (_token == "f" ) return _parseLine_f();
}

void WavefrontObjLoader::_parseLine_v() {
	Tuple4f value(0,0,0,1);

	for (size_t i = 0; i < 4; i++) {
		_nextToken();
		if (_token.size() <= 0) break;
		if (!StringUtil::tryParse(_token, value.data[i])) {
			_error("error read v");
		}
	}

	Tuple3f pos(value.x / value.w
				, value.y / value.w
				, value.z / value.w);
	_tmpPos.emplace_back(pos);
}

void WavefrontObjLoader::_parseLine_vt() {
	Tuple2f value(0,0);

	for (size_t i = 0; i < 2; i++) {
		_nextToken();
		if (_token.size() <= 0) break;
		if (!StringUtil::tryParse(_token, value.data[i])) {
			_error("error read vt");
		}
	}
	_tmpUv.emplace_back(value);
}

void WavefrontObjLoader::_parseLine_vn() {
	Tuple3f value(0,0,0);

	for (size_t i = 0; i < 3; i++) {
		_nextToken();
		if (_token.size() <= 0) break;
		if (!StringUtil::tryParse(_token, value.data[i])) {
			_error("error read vn");
		}
	}
	_tmpNormal.emplace_back(value);
}

void WavefrontObjLoader::_parseLine_f() {
	Vector<int, 64>	face_vi;
	Vector<int, 64>	face_vt;
	Vector<int, 64>	face_vn;

	while (_lineRemain.size() > 0) {
		_nextToken();
		if (_token.size() <= 0) break;

		int vi = 0, vt = 0, vn = 0;

		auto pair = StringUtil::splitByChar(_token, '/');
		if (!StringUtil::tryParse(pair.first, vi)) {
			_error("error read f vi");
		}

		//vt
		pair = StringUtil::splitByChar(pair.second, '/');
		if (pair.first.size()) {
			if (!StringUtil::tryParse(pair.first, vt)) {
				_error("error read f vt");
			}
		}
		//vn
		pair = StringUtil::splitByChar(pair.second, '/');
		if (pair.first.size()) {
			if (!StringUtil::tryParse(pair.first, vn)) {
				_error("error read f vn");
			}
		}

		// index start from 1 and Negative values indicate relative vertex numbers
		if (vi != 0) {
			if (vi > 0) { vi--; }
			else { vi = static_cast<int>(_tmpPos.size()) + vi; }
			face_vi.emplace_back(vi);
		}

		if (vt != 0) {
			if (vt > 0) { vt--; }
			else { vt = static_cast<int>(_tmpUv.size()) + vt; }
			face_vt.emplace_back(vt);
		}
	
		if (vn != 0) {
			if (vn > 0) { vn--; }
			else { vn = static_cast<int>(_tmpNormal.size()) + vn; }
			face_vn.emplace_back(vn);
		}
	}

	if (face_vi.size() < 3) {
		_error("face size < 3");
	}
	
	u32 indicesStart = static_cast<u32>(_outMesh->pos.size());

#if 0 // flip face front/back
	eastl::reverse(face_vi.begin(), face_vi.end());
	eastl::reverse(face_vt.begin(), face_vt.end());
	eastl::reverse(face_vn.begin(), face_vn.end());
#endif

	for (int i = 0; i < face_vi.size(); i++) {
		auto vi = face_vi[i];

		if (vi >= _tmpPos.size()) _error("face vi out of range");
		_outMesh->pos.emplace_back(_tmpPos[vi]);

		if (face_vt.size() >= face_vi.size()) {
			auto vt = face_vt[i];
			if (vt >= _tmpUv.size()) _error("face vt out of range");
			_outMesh->uv[0].emplace_back(_tmpUv[vt]);
		}

		if (face_vn.size() >= face_vn.size()) {
			auto vn = face_vn[i];
			if (vn >= _tmpNormal.size()) _error("face vn out of range");
			_outMesh->normal.emplace_back(_tmpNormal[vn]);
		}
	}

	for (size_t i = 0; i < face_vi.size(); i++) {
		if (i >= 3) {
			_outMesh->indices.emplace_back(static_cast<u32>(indicesStart + 0));
			_outMesh->indices.emplace_back(static_cast<u32>(indicesStart + i - 1));
		}

		u32 vi = static_cast<u32>(indicesStart + i);
		if (vi >= _outMesh->pos.size()) {
			throw SGE_ERROR("vi out of range");
		}
		_outMesh->indices.emplace_back(vi);
	}
}

}