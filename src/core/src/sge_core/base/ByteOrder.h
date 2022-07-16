#pragma once

#include "sge_base.h"

namespace sge {

class ByteOrder {
public:
	
	class Straight {
	public:
		constexpr static u8  get(u8  v) { return v; }
		constexpr static u16 get(u16 v) { return v; }
		constexpr static u32 get(u32 v) { return v; }
		constexpr static u64 get(u64 v) { return v; }

		constexpr static i8  get(i8  v) { return v; }
		constexpr static i16 get(i16 v) { return v; }
		constexpr static i32 get(i32 v) { return v; }
		constexpr static i64 get(i64 v) { return v; }

		constexpr static f32 get(f32 v) { return v; }
		constexpr static f64 get(f64 v) { return v; }
	};

	class ByteSwap {
	public:
		constexpr static u8  get(u8  v) { return v; }
		constexpr static u16 get(u16 v) { return (u16)(v >> 8) | (v << 8); }
		constexpr static u32 get(u32 v) { 
			v = ((v<<8) & 0xFF00FF00UL) | ((v>>8) & 0x00FF00FFUL); 
			return (v>>16) | (v<<16);
		}
		constexpr static u64 get(u64 v) { 
			v = ((v<< 8) & 0xFF00FF00FF00FF00ULL) | ((v>> 8) & 0x00FF00FF00FF00FFULL);
			v = ((v<<16) & 0xFFFF0000FFFF0000ULL) | ((v>>16) & 0x0000FFFF0000FFFFULL);
			return (v>>32) | (v<<32);
		}

		constexpr static i8  get(i8  v) { return static_cast<i8 >(get(static_cast<u8 >(v))); }
		constexpr static i16 get(i16 v) { return static_cast<i16>(get(static_cast<u16>(v))); }
		constexpr static i32 get(i32 v) { return static_cast<i32>(get(static_cast<u32>(v))); }
		constexpr static i64 get(i64 v) { return static_cast<i64>(get(static_cast<u64>(v))); }

		SGE_INLINE static f32 get(f32 v) {
			u32 tmp = get(*reinterpret_cast<const u32*>(&v));
			return *reinterpret_cast<const f32*>(&tmp);
		}
		SGE_INLINE static f64 get(f64 v) {
			u64 tmp = get(*reinterpret_cast<const u64*>(&v));
			return *reinterpret_cast<const f64*>(&tmp);
		}
	};
};

class LittleEndian { // little endian
public:
#if SGE_CPU_ENDIAN_LITTLE
	using ToHost   = ByteOrder::Straight;
	using FromHost = ByteOrder::Straight;
#elif SGE_CPU_ENDIAN_BIG
	using ToHost   = ByteOrder::ByteSwap;
	using FromHost = ByteOrder::ByteSwap;
#endif
};

class BigEndian {
public:
#if SGE_CPU_ENDIAN_LITTLE
	using ToHost   = ByteOrder::ByteSwap;
	using FromHost = ByteOrder::ByteSwap;
#elif SGE_CPU_ENDIAN_BIG
	using ToHost   = ByteOrder::Straight;
	using FromHost = ByteOrder::Straight;
#endif
};

#if SGE_CPU_ENDIAN_LITTLE
	constexpr char32_t FourCC(char a, char b, char c, char d ) {
		return static_cast<char32_t>(a)
		     | static_cast<char32_t>(b) << 8
		     | static_cast<char32_t>(c) << 16
		     | static_cast<char32_t>(d) << 24;
	}
#elif SGE_CPU_ENDIAN_BIG
	constexpr char32_t FourCC(char a, char b, char c, char d ) {
		return static_cast<char32_t>(a) << 24
			 | static_cast<char32_t>(b) << 16
			 | static_cast<char32_t>(c) << 8
			 | static_cast<char32_t>(d);
	}
#else
	#error "Unknown Host Endian"
#endif

constexpr char32_t FourCC(const char (&sz)[5]) { return FourCC(sz[0], sz[1], sz[2], sz[3]); }


} // namespace