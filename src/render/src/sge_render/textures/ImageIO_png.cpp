#include "ImageIO_png.h"

namespace sge {

ImageIO_png::Reader::~Reader() {
	if (_info) {
		png_destroy_info_struct(_png, &_info);
		_info = nullptr;
	}
	if (_png) {
		png_destroy_read_struct(&_png, nullptr, nullptr);
		_png = nullptr;
	}
}

bool ImageIO_png::Reader::error_longjmp_restore_point() {
	// !!! call this function before any libpng C-function that might longjmp()
	// to avoid any C++ destructor or exception try/catch block happen in between

	#if SGE_COMPILER_VC
		#pragma warning(push) 
		#pragma warning(disable: 4611) // interaction between '_setjmp' and C++ object destruction is non-portable
	#endif

	// longjmp() to here from pngReportError()
	return setjmp(png_jmpbuf(_png)) != 0;

	#if SGE_COMPILER_VC
		#pragma warning(pop) 
	#endif
}

void ImageIO_png::Reader::load(Image& img, ByteSpan data, ColorType expectType) {
	_data = data;
	_readPtr = data.data();
	SGE_ASSERT(!_png);

	if (data.size() < 8 || png_sig_cmp(data.data(), 0, 8)) {
		throw SGE_ERROR("not png format");
	}

	_png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (!_png) {
		throw SGE_ERROR("png error read struct");
	}

	_info = png_create_info_struct(_png);
	if (!_info) {
		throw SGE_ERROR("png error create info struct");
	}

	png_set_read_fn( _png, this, &s_onRead );

	//-----
	if (error_longjmp_restore_point()) {
		throw SGE_ERROR("png error read header");
	}
	png_read_info( _png, _info );

	png_uint_32 in_width  = 0;
	png_uint_32 in_height = 0;
	int in_bit = 0;
	int in_color_type = 0;
	int in_interlace = 0;
	bool in_palette_has_alpha = false;

	//-----
	if (error_longjmp_restore_point()) {
		throw SGE_ERROR("png error decompress");
	}
	png_get_IHDR( _png, _info, &in_width, &in_height, &in_bit, &in_color_type, &in_interlace, nullptr, nullptr );	

	auto out_color_type = ColorType::None;
	if (expectType == ColorType::None) {
		if (in_bit == 8) {
			switch (in_color_type) {
				case PNG_COLOR_TYPE_RGB:		out_color_type = ColorType::RGBAb;	break; //DirectX doesn't support RGBb, so convert to RGBAb instead
				case PNG_COLOR_TYPE_RGB_ALPHA:	out_color_type = ColorType::RGBAb;	break;
				case PNG_COLOR_TYPE_GRAY:		out_color_type = ColorType::Lb;		break;
				case PNG_COLOR_TYPE_GRAY_ALPHA:	out_color_type = ColorType::LAb;	break;
				case PNG_COLOR_TYPE_PALETTE:	{
					//get palette transparency table
					png_bytep		trans_alpha = nullptr;
					int				num_trans   = 0;
					png_color_16p	trans_color = nullptr;

					//-----
					if (error_longjmp_restore_point()) {
						throw SGE_ERROR("png error read header color type");
					}
					png_get_tRNS(_png, _info, &trans_alpha, &num_trans, &trans_color);

					out_color_type = ColorType::RGBAb;
					
					if (trans_alpha) {
						in_palette_has_alpha = true;
//						out_color_type = ColorType::RGBAb;
					}else{
//						out_color_type = ColorType::RGBb;
					}
					
				}break;
			}
		} else if (in_bit == 16) {
			switch (in_color_type) {
				case PNG_COLOR_TYPE_RGB:		out_color_type = ColorType::RGBAs;	break; //DirectX doesn't support RGBb, so convert to RGBAb instead
				case PNG_COLOR_TYPE_RGB_ALPHA:	out_color_type = ColorType::RGBAs;	break;
				case PNG_COLOR_TYPE_GRAY:		out_color_type = ColorType::Ls;		break;
				case PNG_COLOR_TYPE_GRAY_ALPHA:	out_color_type = ColorType::LAs;	break;
			}
		} else {
			throw SGE_ERROR("png error unsupported format");
		}
	}

	if (out_color_type == ColorType::None) {
		throw SGE_ERROR("png error unsupported output format");
	}

	//-----
	setReadFilter(out_color_type, in_color_type, in_bit, in_palette_has_alpha);

	int width  = 0;
	int height = 0;
	width  = in_width;
	height = in_height;

	if (width <= 0 || height <= 0) {
		throw SGE_ERROR("png error width <= 0 or height <= 0");
	}

	img.create(out_color_type, width, height);

	Vector<png_bytep, 2048> rows;
	rows.resize(height);
	for (int y = 0; y < height; y++) {
		rows[y] = (png_bytep)img.rowBytes(y).data();
	}

	png_set_rows(_png, _info, rows.data());

	//-----
	if (error_longjmp_restore_point()) {
		throw SGE_ERROR("png error read pixels");
	}
	png_read_image(_png, rows.data());
}

void ImageIO_png::Reader::s_onRead( png_structp png, png_bytep dest, png_size_t len ) {
	auto* p = reinterpret_cast<Reader*>(png_get_io_ptr( png ));
	if( !p ) {
		throw SGE_ERROR("png error read data");
	}
	p->onRead(dest, len);
}

void ImageIO_png::Reader::onRead(png_bytep dest, png_size_t len) {
	size_t n = len;
	if (_readPtr + n > _data.end()) {
		throw SGE_ERROR("png error read data out of range");
	}
	memcpy(dest, _readPtr, n);
	_readPtr += n;
}

void ImageIO_png::Reader::setReadFilter(ColorType out_color_type, int in_color_type, int in_bit, bool in_palette_has_alpha) {
	auto elementType = ColorUtil::elementType(out_color_type);
	if (elementType == ColorElementType::None) throw SGE_ERROR("png error unsupported format");

// 8 <-> 16 bit
	switch (elementType) {
		case ColorElementType::UNorm8: {
			switch (in_bit) {
				case 8:		break;
				case 16:	png_set_scale_16(_png); break;
				default:	throw SGE_ERROR("png error unsupported format");
			}
		}break;

		case ColorElementType::UNorm16: {
			switch (in_bit) {
				case 8:		png_set_expand_16(_png); break;
				case 16:	break;
				default:	throw SGE_ERROR("png error unsupported format");
			}
			#if SGE_CPU_ENDIAN_LITTLE
				png_set_swap(_png);
			#endif
		}break;

		default: throw SGE_ERROR("png error unsupported format");
	}

// palette
	if (in_color_type & PNG_COLOR_MASK_PALETTE) {
		png_set_palette_to_rgb(_png);
	}

// alpha channel
	if (ColorUtil::hasAlpha(out_color_type)) {
		// add alpha channel if missing
		if (in_color_type & PNG_COLOR_MASK_PALETTE) {
			if (!in_palette_has_alpha) {
				png_set_filler(_png, 0xffff, PNG_FILLER_AFTER);
			}
		}
		if (!(in_color_type & PNG_COLOR_MASK_ALPHA)) {
			png_set_filler(_png, 0xffff, PNG_FILLER_AFTER);
		}
	} else {
		png_set_strip_alpha(_png);
	}

// RGB <-> Grayscale
	auto out_color_model = ColorUtil::colorModel(out_color_type);
	switch (out_color_model) {
		case ColorModel::RGB:
		case ColorModel::RGBA: {
			if (!(in_color_type & PNG_COLOR_MASK_COLOR)) png_set_gray_to_rgb(_png);
		}break;

		case ColorModel::L:
		case ColorModel::LA: {
			if (in_color_type & PNG_COLOR_MASK_COLOR) png_set_rgb_to_gray_fixed(_png, 1, -1, -1);
		}break;

		default: throw SGE_ERROR("png error unsupported format");
	}
}

} //namespace
