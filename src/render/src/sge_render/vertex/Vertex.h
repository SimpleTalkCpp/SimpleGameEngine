#pragma once

#include "../Render_Common.h"
#include "../RenderDataType.h"

namespace sge {

// posType          : 8 bit (from 0)
// colorType        : 8 bit (from 8)
// colorCount       : 2 bit (from 16)
// uvType           : 8 bit (from 18)
// uvCount          : 8 bit (from 26)
// normalType       : 8 bit (from 34)
// normalCount      : 2 bit (from 42)
// tangentCount     : 2 bit (from 44)
// binormalCount    : 2 bit (from 46)
// vertexId         : 1 bit (from 48)
enum class VertexType : u64 { None };

struct VertexTypeUtil {
	static constexpr VertexType addPos(VertexType t, RenderDataType posType) {
		return static_cast<VertexType>(static_cast<u64>(t) 
				| static_cast<u64>(posType));
	}

	static constexpr VertexType addColor(VertexType t, RenderDataType colorType, u8 colorCount) {
		return static_cast<VertexType>(static_cast<u64>(t) 
				| (static_cast<u64>(colorType)  << 8 ) 
				| (static_cast<u64>(colorCount) << 16));
	}

	static constexpr VertexType addUv(VertexType t, RenderDataType uvType, u8 uvCount) {
		return static_cast<VertexType>(static_cast<u64>(t) 
				| (static_cast<u64>(uvType)  << 18) 
				| (static_cast<u64>(uvCount) << 26));
	}

	static constexpr VertexType addNormal(VertexType t, RenderDataType normalType, u8 normalCount) {
		return static_cast<VertexType>(static_cast<u64>(t) 
				| (static_cast<u64>(normalType)  << 34) 
				| (static_cast<u64>(normalCount) << 42));
	}

	static constexpr VertexType addTangent(VertexType t, u8 tangentCount) {
		return static_cast<VertexType>(static_cast<u64>(t) 
				| (static_cast<u64>(tangentCount) << 44));
	}
	static constexpr VertexType addBinormal(VertexType t, u8 binormalCount) {
		return static_cast<VertexType>(static_cast<u64>(t) 
				| (static_cast<u64>(binormalCount) << 46));
	}

	static constexpr VertexType addVertexId(VertexType t) {
		return static_cast<VertexType>(static_cast<u64>(t) 
				| (static_cast<u64>(1) << 48));
	}

	static constexpr VertexType make(
		RenderDataType posType, 
		RenderDataType colorType,  u8 colorCount,
		RenderDataType uvType,     u8 uvCount,
		RenderDataType normalType, u8 normalCount, u8 tangentCount, u8 binormalCount)
	{
		VertexType t = addPos(VertexType::None, posType);
		if (colorCount)	 t = addColor	(t, colorType,	colorCount);
		if (uvCount)	 t = addUv		(t, uvType,		uvCount);
		if (normalCount) {
			t = addNormal	(t, normalType,	normalCount);
			t = addTangent	(t, tangentCount);
			t = addBinormal	(t, binormalCount);
		}
		return t;
	}
};

enum class VertexSemantic : u16;
SGE_ENUM_ALL_OPERATOR(VertexSemantic)

using VertexSemanticIndex = u8;

#define VertexSemanticType_ENUM_LIST(E) \
	E(None,)		\
	E(POSITION,)	\
	E(COLOR,)		\
	E(TEXCOORD,)	\
	E(NORMAL,)		\
	E(TANGENT,)		\
	E(BINORMAL,)	\
	E(SV_VertexID,) \
//----
SGE_ENUM_CLASS(VertexSemanticType, u8)

struct VertexSemanticUtil {
	using Semantic = VertexSemantic;
	using Type  = VertexSemanticType;
	using Index = VertexSemanticIndex;

	static constexpr Semantic make(Type type, Index index) {
		return static_cast<Semantic>((enumInt(type) << 8) | index);
	};

	static constexpr u16 _make(Type type, Index index) {
		return static_cast<u16>(make(type, index));
	};

	static constexpr Type	getType (Semantic v) { return static_cast<Type>(enumInt(v) >> 8); }
	static constexpr Index	getIndex(Semantic v) { return static_cast<u8  >(enumInt(v)); }
};

#define VertexSemantic_ENUM_LIST(E) \
	E(None,			= 0) \
	\
	E(POSITION,		= VertexSemanticUtil::_make(VertexSemanticType::POSITION, 0)) \
	\
	E(COLOR0,		= VertexSemanticUtil::_make(VertexSemanticType::COLOR, 0)) \
	E(COLOR1,		= VertexSemanticUtil::_make(VertexSemanticType::COLOR, 1)) \
	E(COLOR2,		= VertexSemanticUtil::_make(VertexSemanticType::COLOR, 2)) \
	E(COLOR3,		= VertexSemanticUtil::_make(VertexSemanticType::COLOR, 3)) \
	\
	E(TEXCOORD0,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 0)) \
	E(TEXCOORD1,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 1)) \
	E(TEXCOORD2,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 2)) \
	E(TEXCOORD3,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 3)) \
	E(TEXCOORD4,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 4)) \
	E(TEXCOORD5,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 5)) \
	E(TEXCOORD6,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 6)) \
	E(TEXCOORD7,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 7)) \
	E(TEXCOORD8,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 8)) \
	E(TEXCOORD9,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 9)) \
	\
	E(TEXCOORD10,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 10)) \
	E(TEXCOORD11,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 11)) \
	E(TEXCOORD12,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 12)) \
	E(TEXCOORD13,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 13)) \
	E(TEXCOORD14,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 14)) \
	E(TEXCOORD15,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 15)) \
	E(TEXCOORD16,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 16)) \
	E(TEXCOORD17,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 17)) \
	E(TEXCOORD18,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 18)) \
	E(TEXCOORD19,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 19)) \
	\
	E(TEXCOORD20,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 20)) \
	E(TEXCOORD21,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 21)) \
	E(TEXCOORD22,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 22)) \
	E(TEXCOORD23,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 23)) \
	E(TEXCOORD24,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 24)) \
	E(TEXCOORD25,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 25)) \
	E(TEXCOORD26,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 26)) \
	E(TEXCOORD27,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 27)) \
	E(TEXCOORD28,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 28)) \
	E(TEXCOORD29,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 29)) \
	\
	E(TEXCOORD30,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 30)) \
	E(TEXCOORD31,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 31)) \
	E(TEXCOORD32,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 32)) \
	E(TEXCOORD33,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 33)) \
	E(TEXCOORD34,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 34)) \
	E(TEXCOORD35,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 35)) \
	E(TEXCOORD36,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 36)) \
	E(TEXCOORD37,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 37)) \
	E(TEXCOORD38,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 38)) \
	E(TEXCOORD39,	= VertexSemanticUtil::_make(VertexSemanticType::TEXCOORD, 39)) \
	\
	E(NORMAL,		= VertexSemanticUtil::_make(VertexSemanticType::NORMAL,   0)) \
	E(TANGENT,		= VertexSemanticUtil::_make(VertexSemanticType::TANGENT,  0)) \
	E(BINORMAL,		= VertexSemanticUtil::_make(VertexSemanticType::BINORMAL, 0)) \
	\
	E(SV_VertexID,	= VertexSemanticUtil::_make(VertexSemanticType::SV_VertexID, 0)) \
//-----
SGE_ENUM_CLASS(VertexSemantic, u16)

struct VertexLayout : public NonCopyable {
	using Semantic = VertexSemantic;
	using DataType = RenderDataType;

	struct Element {
		Semantic	semantic	= Semantic::None;
		u16			offset		= 0;
		DataType	dataType	= DataType::None;
	};

	VertexType type = VertexType::None;
	size_t	stride = 0;
	Vector<Element, 16>	elements;

	template<class VERTEX, class ATTR>
	void addElement(Semantic semantic, ATTR VERTEX::*attr) {
		if (std::is_array<ATTR>()) {
			size_t n = std::extent<ATTR>();
			for (size_t i = 0; i < n; i++) {
				_addElement(semantic + static_cast<int>(i), attr, i);
			}
		} else {
			_addElement(semantic, attr, 0);
		}
	}

	const Element* find(Semantic semantic) const {
		for (auto& e : elements) {
			if (e.semantic == semantic) return &e;
		}
		return nullptr;
	}

private:
	template<class VERTEX, class ATTR>
	void _addElement(Semantic semantic, ATTR VERTEX::*attr, size_t index) {
		auto& o = elements.push_back();
		o.semantic = semantic;
		using A = std::remove_extent<ATTR>::type;
		o.dataType = RenderDataTypeUtil::get<A>();
		o.offset   = static_cast<u16>(memberOffset(attr) + sizeof(A) * index);
	}
};

struct VertexBase {
	using Semantic = VertexSemantic;
	using PosType    = void;
	using ColorType  = void;
	using UvType     = void;
	using NormalType = void;

	static const RenderDataType	kPosType    = RenderDataType::None;
	static const RenderDataType	kColorType  = RenderDataType::None;
	static const RenderDataType	kUvType     = RenderDataType::None;
	static const RenderDataType	kNormalType = RenderDataType::None;

	static const u8 kColorCount		= 0;
	static const u8 kUvCount		= 0;
	static const u8 kNormalCount	= 0;
	static const u8 kTangentCount	= 0;
	static const u8 kBinormalCount	= 0;

	static const VertexType kType = VertexType::None;
};

template<class POS_TYPE>
struct VertexT_Pos : public VertexBase
{
	using PosType = POS_TYPE;
	POS_TYPE pos;

	static const RenderDataType	kPosType	= RenderDataTypeUtil::get<POS_TYPE>();
	static const VertexType		kType		= VertexTypeUtil::addPos(VertexType::None, kPosType);

	static const VertexLayout* s_layout() {
		static const VertexLayout* s = VertexLayoutManager::instance()->getLayout(kType);
		return s;
	}

	static void onRegister(VertexLayout* layout) {
		layout->addElement(Semantic::POSITION, &VertexT_Pos::pos);
	}
};

template<class COLOR_TYPE, u8 COLOR_COUNT, class BASE>
struct VertexT_Color : public BASE
{
	using ColorType = COLOR_TYPE;
	COLOR_TYPE	color[COLOR_COUNT];

	static const RenderDataType	kColorType    = RenderDataTypeUtil::get<COLOR_TYPE>();
	static const u8 kColorCount = COLOR_COUNT;
	static const VertexType kType = VertexTypeUtil::addColor(BASE::kType, kColorType, kColorCount);

	static const VertexLayout* s_layout() {
		static const VertexLayout* s = VertexLayoutManager::instance()->getLayout(kType);
		return s;
	}

	static void onRegister(VertexLayout* layout) {
		BASE::onRegister(layout);
		layout->addElement(Semantic::COLOR0, &VertexT_Color::color);
	}
};

template<class UV_TYPE, u8 UV_COUNT, class BASE>
struct VertexT_Uv : public BASE
{
	using UvType = UV_TYPE;
	UV_TYPE		uv[UV_COUNT];

	static const RenderDataType	kUvType    = RenderDataTypeUtil::get<UV_TYPE>();
	static const u8 kUvCount = UV_COUNT;
	static const VertexType kType = VertexTypeUtil::addUv(BASE::kType, kUvType, kUvCount);

	static const VertexLayout* s_layout() {
		static const VertexLayout* s = VertexLayoutManager::instance()->getLayout(kType);
		return s;
	}

	static void onRegister(VertexLayout* layout) {
		BASE::onRegister(layout);
		layout->addElement(Semantic::TEXCOORD0, &VertexT_Uv::uv);
	}
};

template<class NORMAL_TYPE, u8 NORMAL_COUNT, class BASE>
struct VertexT_Normal : public BASE
{
	using NormalType = NORMAL_TYPE;
	NORMAL_TYPE	normal[NORMAL_COUNT];

	static const RenderDataType	kNormalType    = RenderDataTypeUtil::get<NORMAL_TYPE>();
	static const u8 kNormalCount = NORMAL_COUNT;
	static const VertexType kType = VertexTypeUtil::addNormal(BASE::kType, kNormalType, kNormalCount);

	static const VertexLayout* s_layout() {
		static const VertexLayout* s = VertexLayoutManager::instance()->getLayout(kType);
		return s;
	}

	static void onRegister(VertexLayout* layout) {
		BASE::onRegister(layout);
		layout->addElement(Semantic::NORMAL, &VertexT_Normal::normal);
	}
};

template<class TANGENT_TYPE, u8 TANGENT_COUNT, class BASE>
struct VertexT_Tangent : public BASE
{
	using TangentType = TANGENT_TYPE;
	TANGENT_TYPE	tangent[TANGENT_COUNT];

	static const RenderDataType	kTangentType    = RenderDataTypeUtil::get<TANGENT_TYPE>();
	static const u8 kTangentCount = TANGENT_COUNT;
	static const VertexType kType = VertexTypeUtil::addTangent(BASE::kType, kTangentCount);

	static const VertexLayout* s_layout() {
		static const VertexLayout* s = VertexLayoutManager::instance()->getLayout(kType);
		return s;
	}

	static void onRegister(VertexLayout* layout) {
		static_assert(std::is_same<TangentType, NormalType>::value, "");

		BASE::onRegister(layout);
		layout->addElement(Semantic::TANGENT, &VertexT_Tangent::tangent);
	}
};

template<class BINORMAL_TYPE, u8 BINORMAL_COUNT, class BASE>
struct VertexT_Binormal : public BASE
{
	using BinormalType = BINORMAL_TYPE;
	BINORMAL_TYPE	binormal[BINORMAL_COUNT];

	static const RenderDataType	kBinormalType    = RenderDataTypeUtil::get<BINORMAL_TYPE>();
	static const u8 kBinormalCount = BINORMAL_COUNT;
	static const VertexType kType = VertexTypeUtil::addBinormal(BASE::kType, kBinormalCount);

	static const VertexLayout* s_layout() {
		static const VertexLayout* s = VertexLayoutManager::instance()->getLayout(kType);
		return s;
	}

	static void onRegister(VertexLayout* layout) {
		static_assert(std::is_same<BinormalType, NormalType>::value, "");

		BASE::onRegister(layout);
		layout->addElement(Semantic::BINORMAL, &VertexT_Binormal::binormal);
	}
};

template<class BASE>
struct VertexT_VertexId : public BASE {
	u32 vertexId;

	static const VertexType kType = VertexTypeUtil::addVertexId(BASE::kType);

	static const VertexLayout* s_layout() {
		static const VertexLayout* s = VertexLayoutManager::instance()->getLayout(kType);
		return s;
	}

	static void onRegister(VertexLayout* layout) {
		layout->addElement(Semantic::SV_VertexID, &VertexT_VertexId::vertexId);
	}
};

using Vertex_Pos2f			= VertexT_Pos<Tuple2f>;

using Vertex_Pos			= VertexT_Pos<Tuple3f>;
using Vertex_PosNormal		= VertexT_Normal<Tuple3f, 1, Vertex_Pos>;

using Vertex_PosColor		= VertexT_Color<Color4b, 1, Vertex_Pos>;
using Vertex_PosColorNormal	= VertexT_Normal<Tuple3f, 1, Vertex_PosColor>;

template<u8 UV_COUNT> using Vertex_PosUv				= VertexT_Uv<Tuple2f, UV_COUNT, Vertex_Pos>;
template<u8 UV_COUNT> using Vertex_PosColorUv			= VertexT_Uv<Tuple2f, UV_COUNT, Vertex_PosColor>;

template<u8 UV_COUNT> using Vertex_PosNormalUv			= VertexT_Normal<Tuple3f,   1, Vertex_PosUv<UV_COUNT>>;
template<u8 UV_COUNT> using Vertex_PosColorNormalUv		= VertexT_Normal<Tuple3f,   1, Vertex_PosColorUv<UV_COUNT>>;

template<u8 UV_COUNT> using Vertex_PosTangentUv			= VertexT_Tangent<Tuple3f,  1, Vertex_PosNormalUv<UV_COUNT>>;
template<u8 UV_COUNT> using Vertex_PosColorTangentUv	= VertexT_Tangent<Tuple3f,  1, Vertex_PosColorNormalUv<UV_COUNT>>;

template<u8 UV_COUNT> using Vertex_PosBinormalUv		= VertexT_Binormal<Tuple3f, 1, Vertex_PosTangentUv<UV_COUNT>>;
template<u8 UV_COUNT> using Vertex_PosColorBinormalUv	= VertexT_Binormal<Tuple3f, 1, Vertex_PosColorTangentUv<UV_COUNT>>;

using Vertex_ImGui = VertexT_Color<Color4b, 1, VertexT_Uv<Tuple2f, 1, Vertex_Pos2f>>;

using Vertex_VertexId	= VertexT_VertexId<VertexBase>;

}
