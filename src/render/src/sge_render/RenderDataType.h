#pragma once

namespace sge {

enum class RenderDataType : u8 {
	None,

	Int8,		Int8x2,		Int8x3,		Int8x4,
	Int16,		Int16x2,	Int16x3,	Int16x4,
	Int32,		Int32x2,	Int32x3,	Int32x4,
	Int64,		Int64x2,	Int64x3,	Int64x4,

	UInt8,		UInt8x2,	UInt8x3,	UInt8x4,
	UInt16,		UInt16x2,	UInt16x3,	UInt16x4,
	UInt32,		UInt32x2,	UInt32x3,	UInt32x4,
	UInt64,		UInt64x2,	UInt64x3,	UInt64x4,

	Float16,	Float16x2,	Float16x3,	Float16x4,
	Float32,	Float32x2,	Float32x3,	Float32x4,
	Float64,	Float64x2,	Float64x3,	Float64x4,

	UNorm8,		UNorm8x2,	UNorm8x3,	UNorm8x4,

};

template<class T> constexpr RenderDataType RenderDataType_get();

template<> constexpr RenderDataType RenderDataType_get<void>() { return RenderDataType::None;  }

template<> constexpr RenderDataType RenderDataType_get<i8 >() { return RenderDataType::Int8;  }
template<> constexpr RenderDataType RenderDataType_get<i16>() { return RenderDataType::Int16; }
template<> constexpr RenderDataType RenderDataType_get<i32>() { return RenderDataType::Int32; }
template<> constexpr RenderDataType RenderDataType_get<i64>() { return RenderDataType::Int64; }

template<> constexpr RenderDataType RenderDataType_get<u8 >() { return RenderDataType::UInt8;  }
template<> constexpr RenderDataType RenderDataType_get<u16>() { return RenderDataType::UInt16; }
template<> constexpr RenderDataType RenderDataType_get<u32>() { return RenderDataType::UInt32; }
template<> constexpr RenderDataType RenderDataType_get<u64>() { return RenderDataType::UInt64; }

template<> constexpr RenderDataType RenderDataType_get<f32>() { return RenderDataType::Float32; }
template<> constexpr RenderDataType RenderDataType_get<f64>() { return RenderDataType::Float64; }

template<> constexpr RenderDataType RenderDataType_get<Tuple2f>() { return RenderDataType::Float32x2; }
template<> constexpr RenderDataType RenderDataType_get<Tuple2d>() { return RenderDataType::Float64x2; }

template<> constexpr RenderDataType RenderDataType_get<Tuple3f>() { return RenderDataType::Float32x3; }
template<> constexpr RenderDataType RenderDataType_get<Tuple3d>() { return RenderDataType::Float64x3; }

template<> constexpr RenderDataType RenderDataType_get<Tuple4f>() { return RenderDataType::Float32x4; }
template<> constexpr RenderDataType RenderDataType_get<Tuple4d>() { return RenderDataType::Float64x4; }

template<> constexpr RenderDataType RenderDataType_get<Color4b>() { return RenderDataType::UNorm8x4; }

}