#pragma once

#include "../RenderDataType.h"

namespace sge {

enum class VertexType;

constexpr VertexType VertexType_make(u8 color, u8 uv) {
	u64 o = (u64)color
		|	((u64)uv << 8);

	return static_cast<VertexType>(o);
}

enum class VertexType {
	None,
	Pos				= VertexType_make(0, 0),
	PosColor		= VertexType_make(0, 1),
	PosUvColor		= VertexType_make(1, 1),
	PosUv2Color		= VertexType_make(2, 1),
};


enum class VertexLayout_Semantic;

enum class VertexLayout_SemanticType : u8 {
	None,
	Pos,
	Color,
	TexCoord,
	Normal,
};

SGE_ENUM_ALL_OPERATOR(VertexLayout_Semantic)

constexpr VertexLayout_Semantic VertexLayout_Semantics_make(VertexLayout_SemanticType type, u8 index) {
	return static_cast<VertexLayout_Semantic>((enumInt(type) << 8) | index);
};

enum class VertexLayout_Semantic {
	None = 0,
	Pos			= VertexLayout_Semantics_make(VertexLayout_SemanticType::Pos, 0),

	Color0		= VertexLayout_Semantics_make(VertexLayout_SemanticType::Color, 0),
	Color1		= VertexLayout_Semantics_make(VertexLayout_SemanticType::Color, 1),
	Color2		= VertexLayout_Semantics_make(VertexLayout_SemanticType::Color, 2),
	Color3		= VertexLayout_Semantics_make(VertexLayout_SemanticType::Color, 3),

	TexCoord0	= VertexLayout_Semantics_make(VertexLayout_SemanticType::TexCoord, 0),
	TexCoord1	= VertexLayout_Semantics_make(VertexLayout_SemanticType::TexCoord, 1),
	TexCoord2	= VertexLayout_Semantics_make(VertexLayout_SemanticType::TexCoord, 2),
	TexCoord3	= VertexLayout_Semantics_make(VertexLayout_SemanticType::TexCoord, 3),
	TexCoord4	= VertexLayout_Semantics_make(VertexLayout_SemanticType::TexCoord, 4),
	TexCoord5	= VertexLayout_Semantics_make(VertexLayout_SemanticType::TexCoord, 5),
	TexCoord6	= VertexLayout_Semantics_make(VertexLayout_SemanticType::TexCoord, 6),
	TexCoord7	= VertexLayout_Semantics_make(VertexLayout_SemanticType::TexCoord, 7),
};

struct VertexLayout : public NonCopyable {
	using Semantic = VertexLayout_Semantic;
	using DataType = RenderDataType;

	struct Element {
		Semantic	semantic	= Semantic::None;
		DataType	dataType	= DataType::None;
		intptr_t	offset		= 0;
	};

	VertexType type = VertexType::None;
	size_t	stride = 0;
	Vector_<Element, 16>	elements;

	template<class ATTR>
	void add(Semantic semantic, ATTR* attr) {
		auto& o = elements.push_back();
		o.semantic = semantic;
		o.dataType = RenderDataType_get<ATTR>();
		o.offset   = reinterpret_cast<intptr_t>(attr);
	}
};

template< u8 COLOR_COUNT, u8 UV_COLOR>
struct VertexBase {
	using Semantic = VertexLayout_Semantic;
	const static u8 kColorCount	= COLOR_COUNT;
	const static u8 kUvCount	= UV_COLOR;
	static const VertexType kType = VertexType_make(kColorCount, kUvCount);

	const VertexLayout* layout() {
		static VertexLayout* s = VertexLayoutManager::current()->get(kType);
		return s;
	}
};

template<u8 COLOR_COUNT, u8 UV_COLOR>
struct VertexT_PosUvColor;

template<>
struct VertexT_PosUvColor<0, 0> : public VertexBase<0, 0> {
	Tuple3f pos;

	void onRegister(VertexLayout* layout) {
		layout->add(Semantic::Pos, &pos);
	}
};

template<>
struct VertexT_PosUvColor<0, 1> : public VertexBase<0, 1> {
	Tuple3f pos;
	Color4b color;

	void onRegister(VertexLayout* layout) {
		layout->add(Semantic::Pos,		&pos);
		layout->add(Semantic::Color0,	&color);
	}

};

template<u8 UV_COUNT>
struct VertexT_PosUvColor<UV_COUNT, 1> : public VertexBase<UV_COUNT, 1> {
	Tuple3f pos;
	Color4b color;
	Tuple2f uv[UV_COUNT];

	void onRegister(VertexLayout* layout) {
		layout->add(Semantic::Pos,		&pos);
		layout->add(Semantic::Color0,	&color);

		for (int i = 0; i < UV_COUNT; i++) {
			layout->add(Semantic::TexCoord0 + i,	&uv[i]);
		}
	}
};

using Vertex_Pos			= VertexT_PosUvColor<0, 0>;
using Vertex_PosColor		= VertexT_PosUvColor<0, 1>;
using Vertex_PosUvColor		= VertexT_PosUvColor<1, 1>;
using Vertex_PosUv2Color	= VertexT_PosUvColor<2, 1>;

}