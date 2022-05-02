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

	SNorm8,		SNorm8x2,	SNorm8x3,	SNorm8x4,
	SNorm16,	SNorm16x2,	SNorm16x3,	SNorm16x4,
	SNorm32,	SNorm32x2,	SNorm32x3,	SNorm32x4,

	UNorm8,		UNorm8x2,	UNorm8x3,	UNorm8x4,
	UNorm16,	UNorm16x2,	UNorm16x3,	UNorm16x4,
	UNorm32,	UNorm32x2,	UNorm32x3,	UNorm32x4,
};

struct RenderDataTypeUtil {
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

	template<> static constexpr Type get<Tuple2f>() { return Type::Float32x2; }
	template<> static constexpr Type get<Tuple2d>() { return Type::Float64x2; }

	template<> static constexpr Type get<Tuple3f>() { return Type::Float32x3; }
	template<> static constexpr Type get<Tuple3d>() { return Type::Float64x3; }

	template<> static constexpr Type get<Tuple4f>() { return Type::Float32x4; }
	template<> static constexpr Type get<Tuple4d>() { return Type::Float64x4; }

	template<> static constexpr Type get<Color4b>() { return Type::UNorm8x4; }
};

}