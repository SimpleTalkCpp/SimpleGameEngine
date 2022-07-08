#pragma once

#include "libpng16/png.h"
#include "Image.h"

namespace sge {

struct ImageIO_png {
	class Reader {
	public:
		void load(Image& img, ByteSpan data, ColorType expectType = ColorType::None);

		~Reader();

		static void s_onRead(png_structp png, png_bytep dest, png_size_t len);
		void onRead(png_bytep dest, png_size_t len);

		void setReadFilter(ColorType out_type, int in_type, int in_bit, bool in_palette_has_alpha);

		bool error_longjmp_restore_point();

		png_structp	_png{nullptr};
		png_infop	_info{nullptr};

		ByteSpan	_data;
		const u8*	_readPtr = nullptr;
	};
};

} //namespace
