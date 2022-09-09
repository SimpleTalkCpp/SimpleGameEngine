#pragma once

#include <sge_render/RenderDataType.h>

namespace sge {

struct ImageInfo {
public:
	Vec2i		size {0,0};
	int			strideInBytes = 0;
	int			mipmapCount = 1;
	int			pixelSizeInBytes() const { return ColorUtil::pixelSizeInBytes(colorType); }
	ColorType	colorType = ColorType::None;
};


//! Image for any color type
class Image : public NonCopyable {
public:
	using Info = ImageInfo;

	Image() = default;
	Image(Image && r);

	void operator=(Image && r);

					void		clear();

					void		loadFile	(StrView filename);

					void		loadPngFile	(StrView filename);
					void		loadPngMem	(ByteSpan data);

					void		loadDdsFile	(StrView filename);
					void		loadDdsMem	(ByteSpan data);

					void		create		(ColorType colorType, int width, int height);
					void		create		(ColorType colorType, int width, int height, int strideInBytes);
					void		create		(ColorType colorType, int width, int height, int strideInBytes, int mipmapCount, size_t dataSizeInBytes);

	SGE_INLINE	const Info&		info			() const { return _info; }
	SGE_INLINE	const Vec2i&	size			() const { return _info.size; }
	SGE_INLINE	int				strideInBytes	() const { return _info.strideInBytes; }
	SGE_INLINE	int				width			() const { return _info.size.x; }
	SGE_INLINE	int				height			() const { return _info.size.y; }
	SGE_INLINE	ColorType		colorType		() const { return _info.colorType; }

	template<class COLOR> SGE_INLINE	Span<      COLOR>	row(int y)			{ _checkType(COLOR::kColorType); return row_noCheck<COLOR>(y); }
	template<class COLOR> SGE_INLINE	Span<const COLOR>	row(int y) const	{ _checkType(COLOR::kColorType); return row_noCheck<COLOR>(y); }

	template<class COLOR> SGE_INLINE	const COLOR&		pixel(int x, int y) const	{ return row<COLOR>(y)[x]; }
	template<class COLOR> SGE_INLINE		  COLOR&		pixel(int x, int y)			{ return row<COLOR>(y)[x]; }

	template<class COLOR> SGE_INLINE Span<      COLOR>		row_noCheck(int y)			{ return Span<      COLOR>(reinterpret_cast<      COLOR*>(rowBytes(y).data()), _info.size.x); }
	template<class COLOR> SGE_INLINE Span<const COLOR>		row_noCheck(int y) const	{ return Span<const COLOR>(reinterpret_cast<const COLOR*>(rowBytes(y).data()), _info.size.x); }

	template<class COLOR> void fill(const COLOR& color);

	SGE_INLINE	Span<u8>		rowBytes(int y)			{ return Span<      u8>(&_pixelData[y * _info.strideInBytes], _info.size.x * _info.pixelSizeInBytes()); }
	SGE_INLINE	Span<const u8>	rowBytes(int y) const	{ return Span<const u8>(&_pixelData[y * _info.strideInBytes], _info.size.x * _info.pixelSizeInBytes()); }

	const void* dataPtr() const { return _pixelData.data(); }

	void copyToPixelData(ByteSpan src) { _pixelData.assign(src.begin(), src.end()); }

	void copy(const Image& src) {
		_info = src._info;
		_pixelData = src._pixelData;
	}

private:
	void _create(ColorType colorType, int width, int height, int strideInBytes, int mipmapCount, size_t dataSizeInBytes);
	void _checkType(ColorType colorType) const {
		if (colorType != _info.colorType) throw SGE_ERROR("Invalid ColorType");
	}

	Info _info;
	Vector<u8>	_pixelData;
};

template<class COLOR> inline
void Image::fill(const COLOR& color) {
	_checkType(COLOR::kColorType);
	for (int y=0; y<_info.size.y; y++) {
		for (auto& p : row_noCheck<COLOR>(y)) {
			p = color;
		}
	}
}

} // namespace