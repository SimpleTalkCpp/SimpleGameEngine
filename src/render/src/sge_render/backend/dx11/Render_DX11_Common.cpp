#include "Render_DX11_Common.h"

namespace sge {

VertexSemanticType DX11Util::parseDxSemanticName(StrView s) {
	VertexSemanticType v;

	if (s == "SV_POSITION") {
		return VertexSemanticType::POSITION;
	}

	if (!enumTryParse(v, s)) {
		throw SGE_ERROR("unknown VertexLayout_SemanticType {}", s);
	}
	return v;
}

const char* DX11Util::getDxSemanticName(VertexSemanticType v) {
	const char* s = enumStr(v);
	if (!s) {
		throw SGE_ERROR("unknown VertexLayout_SemanticType {}", v);
	}
	return s;
}

}