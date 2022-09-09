#pragma once

#include "Color.h"

namespace sge {

struct ColorUtil {
	ColorUtil() = delete;

	static constexpr int pixelSizeInBytes(ColorType t);
	static constexpr int bytesPerPixelBlock(ColorType type);

	static constexpr ColorElementType	elementType(ColorType t) { return static_cast<ColorElementType>(enumInt(t) & 0xff); }
	static constexpr ColorModel			colorModel( ColorType t) { return static_cast<ColorModel>((enumInt(t) >> 8) & 0xff); }

	static constexpr bool hasAlpha(ColorType t);
};

constexpr bool ColorUtil::hasAlpha(ColorType t) {
	auto model = colorModel(t);
	switch (model) {
		case ColorModel::RGBA: return true;
	}
	return false;
}

constexpr int ColorUtil::pixelSizeInBytes(ColorType t) {
	switch (t) {
		case ColorType::RGBAb: return sizeof(ColorRGBAb);
		case ColorType::RGBAs: return sizeof(ColorRGBAs);
		case ColorType::RGBAf: return sizeof(ColorRGBAf);

		case ColorType::Rb: return sizeof(ColorRb);
		case ColorType::Rs: return sizeof(ColorRs);
		case ColorType::Rf: return sizeof(ColorRf);

		case ColorType::Lb: return sizeof(ColorLb);
		case ColorType::Ls: return sizeof(ColorLs);
		case ColorType::Lf: return sizeof(ColorLf);
	}

	SGE_ASSERT(false);
	return 0;
}

constexpr int ColorUtil::bytesPerPixelBlock(ColorType type) {
	switch (type) {
		#define E(T) \
			case ColorType::T: return Color##T::kBytesPerPixelBlock;
		//----------
			E(BC1)
			E(BC2)
			E(BC3)
			E(BC4)
			E(BC5)
			E(BC6h)
			E(BC7)
		#undef E
	}
	return 0;
}

} // namespace