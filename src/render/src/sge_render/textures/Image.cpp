#include "Image.h"
#include "ImageIO_png.h"

namespace sge {

void Image::clear() {
	_info = ImageInfo();
	_pixelData.clear();
}

void Image::loadFile(StrView filename) {
	auto ext = FilePath::extension(filename);
	if (0 == StringUtil::ignoreCaseCompare(ext, "png")) {
		return loadPngFile(filename);
	}

	throw SGE_ERROR("unsupported image file format {}", ext);
}

void Image::loadPngFile(StrView filename) {
	MemMapFile mm;
	mm.open(filename);
	loadPngMem(mm);
}

void Image::loadPngMem(ByteSpan data) {
	ImageIO_png::Reader r;
	r.load(*this, data);
}

void Image::create(ColorType colorType, int width, int height) {
	create(colorType, width, height, width * ColorUtil::pixelSizeInBytes(colorType));
}

void Image::create(ColorType colorType, int width, int height, int strideInBytes) {
	_create(colorType, width, height, strideInBytes, 1, strideInBytes * height);
}

void Image::_create(ColorType colorType, int width, int height, int strideInBytes, int mipmapCount, int dataSizeInBytes) {
	_info.colorType = colorType;
	_info.size.set(width, height);
	_info.strideInBytes	= strideInBytes;
	_info.mipmapCount	= mipmapCount;

	try{
		_pixelData.clear();
		_pixelData.resize(dataSizeInBytes);
	}catch(...) {
		clear();
		throw;
	}
}

}