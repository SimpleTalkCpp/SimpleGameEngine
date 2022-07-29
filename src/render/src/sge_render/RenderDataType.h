#pragma once

#include <sge_core.h>

namespace sge {

class Texture1D;
class Texture2D;
class Texture3D;
class TextureCube;

#define RenderDataType_ENUM_LIST(E) \
	E(None,) \
	\
	E(Int8,)	E(Int8x2,)		E(Int8x3,)		E(Int8x4,)		\
	E(Int16,)	E(Int16x2,)		E(Int16x3,)		E(Int16x4,)		\
	E(Int32,)	E(Int32x2,)		E(Int32x3,)		E(Int32x4,)		\
	E(Int64,)	E(Int64x2,)		E(Int64x3,)		E(Int64x4,)		\
	\
	E(UInt8,)	E(UInt8x2,)		E(UInt8x3,)		E(UInt8x4,)		\
	E(UInt16,)	E(UInt16x2,)	E(UInt16x3,)	E(UInt16x4,)	\
	E(UInt32,)	E(UInt32x2,)	E(UInt32x3,)	E(UInt32x4,)	\
	E(UInt64,)	E(UInt64x2,)	E(UInt64x3,)	E(UInt64x4,)	\
	\
	E(Float16,)	E(Float16x2,)	E(Float16x3,)	E(Float16x4,)	\
	E(Float32,)	E(Float32x2,)	E(Float32x3,)	E(Float32x4,)	\
	E(Float64,)	E(Float64x2,)	E(Float64x3,)	E(Float64x4,)	\
	\
	E(SNorm8,)	E(SNorm8x2,)	E(SNorm8x3,)	E(SNorm8x4,)	\
	E(SNorm16,)	E(SNorm16x2,)	E(SNorm16x3,)	E(SNorm16x4,)	\
	E(SNorm32,)	E(SNorm32x2,)	E(SNorm32x3,)	E(SNorm32x4,)	\
	\
	E(UNorm8,)	E(UNorm8x2,)	E(UNorm8x3,)	E(UNorm8x4,)	\
	E(UNorm16,)	E(UNorm16x2,)	E(UNorm16x3,)	E(UNorm16x4,)	\
	E(UNorm32,)	E(UNorm32x2,)	E(UNorm32x3,)	E(UNorm32x4,)	\
	\
	E(Float32_4x4,) \
	\
	E(Texture1D,)		E(Texture2D,)		E(Texture3D,)		E(TextureCube,) \
	E(Texture1DArray,)	E(Texture2DArray,)	E(Texture3DArray,)	E(TextureCubeArray,) \

//----
SGE_ENUM_CLASS(RenderDataType, u8)

struct RenderDataTypeUtil {
	RenderDataTypeUtil() = delete;
	
	using Type = RenderDataType;

	template<class T> static constexpr Type get();

	template<> static constexpr Type get<void>() { return Type::None;  }

	template<> static constexpr Type get<i8 >() { return Type::Int8;  }
	template<> static constexpr Type get<i16>() { return Type::Int16; }
	template<> static constexpr Type get<i32>() { return Type::Int32; }
	template<> static constexpr Type get<i64>() { return Type::Int64; }

	template<> static constexpr Type get<u8 >() { return Type::UInt8;  }
	template<> static constexpr Type get<u16>() { return Type::UInt16; }
	template<> static constexpr Type get<u32>() { return Type::UInt32; }
	template<> static constexpr Type get<u64>() { return Type::UInt64; }

	template<> static constexpr Type get<f32>() { return Type::Float32; }
	template<> static constexpr Type get<f64>() { return Type::Float64; }

	template<> static constexpr Type get<Tuple2i>() { return Type::Int32x2; }
	template<> static constexpr Type get<Tuple2f>() { return Type::Float32x2; }
	template<> static constexpr Type get<Tuple2d>() { return Type::Float64x2; }

	template<> static constexpr Type get<Tuple3i>() { return Type::Int32x3; }
	template<> static constexpr Type get<Tuple3f>() { return Type::Float32x3; }
	template<> static constexpr Type get<Tuple3d>() { return Type::Float64x3; }

	template<> static constexpr Type get<Tuple4i>() { return Type::Int32x4; }
	template<> static constexpr Type get<Tuple4f>() { return Type::Float32x4; }
	template<> static constexpr Type get<Tuple4d>() { return Type::Float64x4; }

	template<> static constexpr Type get<Mat4f>() { return Type::Float32_4x4; }

	template<> static constexpr Type get<Color4b>() { return Type::UNorm8x4; }

	template<> static constexpr Type get<Texture1D>()	{ return Type::Texture1D; }
	template<> static constexpr Type get<Texture2D>()	{ return Type::Texture2D; }
	template<> static constexpr Type get<Texture3D>()	{ return Type::Texture3D; }
	template<> static constexpr Type get<TextureCube>()	{ return Type::TextureCube; }
};

}
