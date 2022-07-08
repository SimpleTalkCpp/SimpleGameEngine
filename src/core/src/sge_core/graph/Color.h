#pragma once

#include <sge_core/math/Tuple4.h>

namespace sge {

#define ColorModel_ENUM_LIST(E) \
	E(None,) \
	E(R,) \
	E(RG,) \
	E(RGB,) \
	E(RGBA,) \
	E(L,)  /* Luminance */ \
	E(LA,) /* Luminance Alpha */ \
	E(HSV,) \
	E(BlockCompression,) \
//----
SGE_ENUM_CLASS(ColorModel, u8)

#define ColorElementType_ENUM_LIST(E) \
	E(None,) \
	E(UNorm8,) \
	E(UNorm16,) \
	E(Float16,) \
	E(Float32,) \
	E(Float64,) \
//-------
SGE_ENUM_CLASS(ColorElementType, u8);

template<class T> constexpr ColorElementType ColorElementType_get();
template<> constexpr ColorElementType ColorElementType_get<u8 >() { return ColorElementType::UNorm8; }
template<> constexpr ColorElementType ColorElementType_get<u16>() { return ColorElementType::UNorm16; }
//template<> constexpr ColorElementType ColorElementType_get<f16>() { return ColorElementType::Float16; }
template<> constexpr ColorElementType ColorElementType_get<f32>() { return ColorElementType::Float32; }
template<> constexpr ColorElementType ColorElementType_get<f64>() { return ColorElementType::Float64; }

#define ColorCompressType_ENUM_LIST(E) \
	E(None,) \
	E(BC1,) \
	E(BC2,) \
	E(BC3,) \
	E(BC4,) \
	E(BC5,) \
	E(BC6h,) \
	E(BC7,) \
//-------
SGE_ENUM_CLASS(ColorCompressType, u8);


enum class ColorType : u16;

constexpr ColorType ColorType_make(ColorModel model, ColorElementType elem) {
	return static_cast<ColorType>(
			  (static_cast<u32>(model) << 8)
			| (static_cast<u32>(elem))
			);
}

constexpr ColorType ColorType_make(ColorModel model, ColorCompressType compress) {
	return static_cast<ColorType>(
			  (static_cast<u32>(model) << 8)
			| (static_cast<u32>(compress))
			);
}

#define ColorType_ENUM_LIST(E) \
	E(None,) \
	E(Rb,		= ColorType_make(ColorModel::R,		ColorElementType::UNorm8 )) \
	E(Rs,		= ColorType_make(ColorModel::R,		ColorElementType::UNorm16)) \
	E(Rh,		= ColorType_make(ColorModel::R,		ColorElementType::Float16)) \
	E(Rf,		= ColorType_make(ColorModel::R,		ColorElementType::Float32)) \
	\
	E(Lb,		= ColorType_make(ColorModel::L,		ColorElementType::UNorm8 )) \
	E(Ls,		= ColorType_make(ColorModel::L,		ColorElementType::UNorm16)) \
	E(Lh,		= ColorType_make(ColorModel::L,		ColorElementType::Float16)) \
	E(Lf,		= ColorType_make(ColorModel::L,		ColorElementType::Float32)) \
	\
	E(LAb,		= ColorType_make(ColorModel::LA,	ColorElementType::UNorm8 )) \
	E(LAs,		= ColorType_make(ColorModel::LA,	ColorElementType::UNorm16)) \
	E(LAh,		= ColorType_make(ColorModel::LA,	ColorElementType::Float16)) \
	E(LAf,		= ColorType_make(ColorModel::LA,	ColorElementType::Float32)) \
	\
	E(RGb,		= ColorType_make(ColorModel::RG,	ColorElementType::UNorm8 )) \
	E(RGs,		= ColorType_make(ColorModel::RG,	ColorElementType::UNorm16)) \
	E(RGh,		= ColorType_make(ColorModel::RG,	ColorElementType::Float16)) \
	E(RGf,		= ColorType_make(ColorModel::RG,	ColorElementType::Float32)) \
	\
	E(RGBb,		= ColorType_make(ColorModel::RGB,	ColorElementType::UNorm8 )) \
	E(RGBs,		= ColorType_make(ColorModel::RGB,	ColorElementType::UNorm16)) \
	E(RGBh,		= ColorType_make(ColorModel::RGB,	ColorElementType::Float16)) \
	E(RGBf,		= ColorType_make(ColorModel::RGB,	ColorElementType::Float32)) \
	\
	E(RGBAb,	= ColorType_make(ColorModel::RGBA,	ColorElementType::UNorm8 )) \
	E(RGBAs,	= ColorType_make(ColorModel::RGBA,	ColorElementType::UNorm16)) \
	E(RGBAh,	= ColorType_make(ColorModel::RGBA,	ColorElementType::Float16)) \
	E(RGBAf,	= ColorType_make(ColorModel::RGBA,	ColorElementType::Float32)) \
	\
	E(BC1,		= ColorType_make(ColorModel::BlockCompression, ColorCompressType::BC1 )) \
	E(BC2,		= ColorType_make(ColorModel::BlockCompression, ColorCompressType::BC2 )) \
	E(BC3,		= ColorType_make(ColorModel::BlockCompression, ColorCompressType::BC3 )) \
	E(BC4,		= ColorType_make(ColorModel::BlockCompression, ColorCompressType::BC4 )) \
	E(BC5,		= ColorType_make(ColorModel::BlockCompression, ColorCompressType::BC5 )) \
	E(BC6h,		= ColorType_make(ColorModel::BlockCompression, ColorCompressType::BC6h)) \
	E(BC7,		= ColorType_make(ColorModel::BlockCompression, ColorCompressType::BC7 )) \
//----
SGE_ENUM_CLASS(ColorType, u16)

template<class T>
struct ColorR {
	using ElementType = T;
	static const size_t kElementCount = 1;
	static constexpr bool hasAlpha = false;
	static constexpr ColorType kColorType = ColorType_make(ColorModel::R, ColorElementType_get<T>());

	union {
		struct { T r; };
		T data[kElementCount];
	};

	ColorR() = default;
	ColorR(const T& r_)
		: r(r_) {}
};

template<class T>
struct ColorRG {
	using ElementType = T;
	static const size_t kElementCount = 2;
	static constexpr bool hasAlpha = false;
	static constexpr ColorType kColorType = ColorType_make(ColorModel::RG, ColorElementType_get<T>());

	union {
		struct { T r, g; };
		T data[kElementCount];
	};

	ColorRG() = default;
	ColorRG(const T& r_, const T& g_)
		: r(r_), g(g_) {}

};

template<class T>
struct ColorRGB {
	using ElementType = T;
	static const size_t kElementCount = 3;
	static constexpr bool hasAlpha = false;
	static constexpr ColorType kColorType = ColorType_make(ColorModel::RGB, ColorElementType_get<T>());

	union {
		struct { T r, g, b; };
		T data[kElementCount];
	};

	ColorRGB() = default;
	ColorRGB(const T& r_, const T& g_, const T& b_)
		: r(r_), g(g_), b(b_) {}

};

template<class T>
struct ColorRGBA {
	using ElementType = T;
	static const size_t kElementCount = 4;
	static constexpr bool hasAlpha = true;
	static constexpr ColorType kColorType = ColorType_make(ColorModel::RGBA, ColorElementType_get<T>());

	union {
		struct { T r, g, b, a; };
		T data[kElementCount];
	};

	ColorRGBA() = default;
	ColorRGBA(const T& r_, const T& g_, const T& b_, const T& a_)
		: r(r_), g(g_), b(b_), a(a_) {}

	ColorRGB<T> rgb() const { return ColorRGB(r,g,b); }

	Tuple4<T>	toTuple() const { return Tuple4<T>(r,g,b,a); }
	operator Tuple4<T>() const { return toTuple(); }

	void set(const T& r_, const T& g_, const T& b_, const T& a_) {
		r = r_;
		g = g_;
		b = b_;
		a = a_;
	}

	template<class SE>
	void onJson(SE& se) {
		SGE_NAMED_IO(se, r);
		SGE_NAMED_IO(se, g);
		SGE_NAMED_IO(se, b);
		SGE_NAMED_IO(se, a);
	}
};

using ColorRGBAf = ColorRGBA<float>;
using ColorRGBAb = ColorRGBA<u8>;
using ColorRGBAs = ColorRGBA<u16>;

using Color4f = ColorRGBAf;
using Color4b = ColorRGBAb;

}