#pragma once

#include "Color.h"

namespace sge {

struct ColorUtil {
	ColorUtil() = delete;

	static int pixelSizeInBytes(ColorType t);
};

inline
int ColorUtil::pixelSizeInBytes(ColorType t) {
	switch (t) {
		case ColorType::RGBAb: return sizeof(ColorRGBAb);
	}

	SGE_ASSERT(false);
	return 0;
}

} // namespace