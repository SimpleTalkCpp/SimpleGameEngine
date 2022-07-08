#pragma once

#include "Color.h"

namespace sge {

struct ColorUtil {
	ColorUtil() = delete;

	static constexpr int pixelSizeInBytes(ColorType t);

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
	}

	SGE_ASSERT(false);
	return 0;
}

} // namespace