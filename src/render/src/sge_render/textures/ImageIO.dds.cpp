#include "ImageIO.dds.h"

namespace sge {

enum DXGI_FORMAT : uint32_t { 
	DXGI_FORMAT_UNKNOWN                     = 0,
	DXGI_FORMAT_R32G32B32A32_TYPELESS       = 1,
	DXGI_FORMAT_R32G32B32A32_FLOAT          = 2,
	DXGI_FORMAT_R32G32B32A32_UINT           = 3,
	DXGI_FORMAT_R32G32B32A32_SINT           = 4,
	DXGI_FORMAT_R32G32B32_TYPELESS          = 5,
	DXGI_FORMAT_R32G32B32_FLOAT             = 6,
	DXGI_FORMAT_R32G32B32_UINT              = 7,
	DXGI_FORMAT_R32G32B32_SINT              = 8,
	DXGI_FORMAT_R16G16B16A16_TYPELESS       = 9,
	DXGI_FORMAT_R16G16B16A16_FLOAT          = 10,
	DXGI_FORMAT_R16G16B16A16_UNORM          = 11,
	DXGI_FORMAT_R16G16B16A16_UINT           = 12,
	DXGI_FORMAT_R16G16B16A16_SNORM          = 13,
	DXGI_FORMAT_R16G16B16A16_SINT           = 14,
	DXGI_FORMAT_R32G32_TYPELESS             = 15,
	DXGI_FORMAT_R32G32_FLOAT                = 16,
	DXGI_FORMAT_R32G32_UINT                 = 17,
	DXGI_FORMAT_R32G32_SINT                 = 18,
	DXGI_FORMAT_R32G8X24_TYPELESS           = 19,
	DXGI_FORMAT_D32_FLOAT_S8X24_UINT        = 20,
	DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS    = 21,
	DXGI_FORMAT_X32_TYPELESS_G8X24_UINT     = 22,
	DXGI_FORMAT_R10G10B10A2_TYPELESS        = 23,
	DXGI_FORMAT_R10G10B10A2_UNORM           = 24,
	DXGI_FORMAT_R10G10B10A2_UINT            = 25,
	DXGI_FORMAT_R11G11B10_FLOAT             = 26,
	DXGI_FORMAT_R8G8B8A8_TYPELESS           = 27,
	DXGI_FORMAT_R8G8B8A8_UNORM              = 28,
	DXGI_FORMAT_R8G8B8A8_UNORM_SRGB         = 29,
	DXGI_FORMAT_R8G8B8A8_UINT               = 30,
	DXGI_FORMAT_R8G8B8A8_SNORM              = 31,
	DXGI_FORMAT_R8G8B8A8_SINT               = 32,
	DXGI_FORMAT_R16G16_TYPELESS             = 33,
	DXGI_FORMAT_R16G16_FLOAT                = 34,
	DXGI_FORMAT_R16G16_UNORM                = 35,
	DXGI_FORMAT_R16G16_UINT                 = 36,
	DXGI_FORMAT_R16G16_SNORM                = 37,
	DXGI_FORMAT_R16G16_SINT                 = 38,
	DXGI_FORMAT_R32_TYPELESS                = 39,
	DXGI_FORMAT_D32_FLOAT                   = 40,
	DXGI_FORMAT_R32_FLOAT                   = 41,
	DXGI_FORMAT_R32_UINT                    = 42,
	DXGI_FORMAT_R32_SINT                    = 43,
	DXGI_FORMAT_R24G8_TYPELESS              = 44,
	DXGI_FORMAT_D24_UNORM_S8_UINT           = 45,
	DXGI_FORMAT_R24_UNORM_X8_TYPELESS       = 46,
	DXGI_FORMAT_X24_TYPELESS_G8_UINT        = 47,
	DXGI_FORMAT_R8G8_TYPELESS               = 48,
	DXGI_FORMAT_R8G8_UNORM                  = 49,
	DXGI_FORMAT_R8G8_UINT                   = 50,
	DXGI_FORMAT_R8G8_SNORM                  = 51,
	DXGI_FORMAT_R8G8_SINT                   = 52,
	DXGI_FORMAT_R16_TYPELESS                = 53,
	DXGI_FORMAT_R16_FLOAT                   = 54,
	DXGI_FORMAT_D16_UNORM                   = 55,
	DXGI_FORMAT_R16_UNORM                   = 56,
	DXGI_FORMAT_R16_UINT                    = 57,
	DXGI_FORMAT_R16_SNORM                   = 58,
	DXGI_FORMAT_R16_SINT                    = 59,
	DXGI_FORMAT_R8_TYPELESS                 = 60,
	DXGI_FORMAT_R8_UNORM                    = 61,
	DXGI_FORMAT_R8_UINT                     = 62,
	DXGI_FORMAT_R8_SNORM                    = 63,
	DXGI_FORMAT_R8_SINT                     = 64,
	DXGI_FORMAT_A8_UNORM                    = 65,
	DXGI_FORMAT_R1_UNORM                    = 66,
	DXGI_FORMAT_R9G9B9E5_SHAREDEXP          = 67,
	DXGI_FORMAT_R8G8_B8G8_UNORM             = 68,
	DXGI_FORMAT_G8R8_G8B8_UNORM             = 69,
	DXGI_FORMAT_BC1_TYPELESS                = 70,
	DXGI_FORMAT_BC1_UNORM                   = 71,
	DXGI_FORMAT_BC1_UNORM_SRGB              = 72,
	DXGI_FORMAT_BC2_TYPELESS                = 73,
	DXGI_FORMAT_BC2_UNORM                   = 74,
	DXGI_FORMAT_BC2_UNORM_SRGB              = 75,
	DXGI_FORMAT_BC3_TYPELESS                = 76,
	DXGI_FORMAT_BC3_UNORM                   = 77,
	DXGI_FORMAT_BC3_UNORM_SRGB              = 78,
	DXGI_FORMAT_BC4_TYPELESS                = 79,
	DXGI_FORMAT_BC4_UNORM                   = 80,
	DXGI_FORMAT_BC4_SNORM                   = 81,
	DXGI_FORMAT_BC5_TYPELESS                = 82,
	DXGI_FORMAT_BC5_UNORM                   = 83,
	DXGI_FORMAT_BC5_SNORM                   = 84,
	DXGI_FORMAT_B5G6R5_UNORM                = 85,
	DXGI_FORMAT_B5G5R5A1_UNORM              = 86,
	DXGI_FORMAT_B8G8R8A8_UNORM              = 87,
	DXGI_FORMAT_B8G8R8X8_UNORM              = 88,
	DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM  = 89,
	DXGI_FORMAT_B8G8R8A8_TYPELESS           = 90,
	DXGI_FORMAT_B8G8R8A8_UNORM_SRGB         = 91,
	DXGI_FORMAT_B8G8R8X8_TYPELESS           = 92,
	DXGI_FORMAT_B8G8R8X8_UNORM_SRGB         = 93,
	DXGI_FORMAT_BC6H_TYPELESS               = 94,
	DXGI_FORMAT_BC6H_UF16                   = 95,
	DXGI_FORMAT_BC6H_SF16                   = 96,
	DXGI_FORMAT_BC7_TYPELESS                = 97,
	DXGI_FORMAT_BC7_UNORM                   = 98,
	DXGI_FORMAT_BC7_UNORM_SRGB              = 99,
	DXGI_FORMAT_AYUV                        = 100,
	DXGI_FORMAT_Y410                        = 101,
	DXGI_FORMAT_Y416                        = 102,
	DXGI_FORMAT_NV12                        = 103,
	DXGI_FORMAT_P010                        = 104,
	DXGI_FORMAT_P016                        = 105,
	DXGI_FORMAT_420_OPAQUE                  = 106,
	DXGI_FORMAT_YUY2                        = 107,
	DXGI_FORMAT_Y210                        = 108,
	DXGI_FORMAT_Y216                        = 109,
	DXGI_FORMAT_NV11                        = 110,
	DXGI_FORMAT_AI44                        = 111,
	DXGI_FORMAT_IA44                        = 112,
	DXGI_FORMAT_P8                          = 113,
	DXGI_FORMAT_A8P8                        = 114,
	DXGI_FORMAT_B4G4R4A4_UNORM              = 115,
	DXGI_FORMAT_P208                        = 130,
	DXGI_FORMAT_V208                        = 131,
	DXGI_FORMAT_V408                        = 132,
	DXGI_FORMAT_FORCE_UINT                  = 0xffffffff
};

enum D3D10_RESOURCE_DIMENSION : uint32_t { 
	D3D10_RESOURCE_DIMENSION_UNKNOWN    = 0,
	D3D10_RESOURCE_DIMENSION_BUFFER     = 1,
	D3D10_RESOURCE_DIMENSION_TEXTURE1D  = 2,
	D3D10_RESOURCE_DIMENSION_TEXTURE2D  = 3,
	D3D10_RESOURCE_DIMENSION_TEXTURE3D  = 4
};

struct DDS_PIXELFORMAT {
	uint32_t dwSize;
	uint32_t dwFlags;
	uint32_t dwFourCC;
	uint32_t dwRGBBitCount;
	uint32_t dwRBitMask;
	uint32_t dwGBitMask;
	uint32_t dwBBitMask;
	uint32_t dwABitMask;
};

#define DDS_FOURCC      0x00000004  // DDPF_FOURCC
#define DDS_RGB         0x00000040  // DDPF_RGB
#define DDS_RGBA        0x00000041  // DDPF_RGB | DDPF_ALPHAPIXELS
#define DDS_LUMINANCE   0x00020000  // DDPF_LUMINANCE
#define DDS_LUMINANCEA  0x00020001  // DDPF_LUMINANCE | DDPF_ALPHAPIXELS
#define DDS_ALPHAPIXELS 0x00000001  // DDPF_ALPHAPIXELS
#define DDS_ALPHA       0x00000002  // DDPF_ALPHA
#define DDS_PAL8        0x00000020  // DDPF_PALETTEINDEXED8
#define DDS_PAL8A       0x00000021  // DDPF_PALETTEINDEXED8 | DDPF_ALPHAPIXELS
#define DDS_BUMPDUDV    0x00080000  // DDPF_BUMPDUDV


struct DDS_HEADER {
	uint32_t dwSize;
	uint32_t dwHeaderFlags;
	uint32_t dwHeight;
	uint32_t dwWidth;
	uint32_t dwPitchOrLinearSize;
	uint32_t dwDepth; // only if DDS_HEADER_FLAGS_VOLUME is set in dwHeaderFlags
	uint32_t dwMipMapCount;
	uint32_t dwReserved1[11];
	DDS_PIXELFORMAT ddspf;
	uint32_t dwSurfaceFlags;
	uint32_t dwCubemapFlags;
	uint32_t dwReserved2[3];
};

struct DDS_HEADER_DXT10
{
	DXGI_FORMAT              dxgiFormat;
	D3D10_RESOURCE_DIMENSION resourceDimension;
	uint32_t                 miscFlag;
	uint32_t                 arraySize;
	uint32_t                 miscFlags2;
};

void ImageIO_dds::Reader::_readHeader(BinDeserializer& de, DDS_HEADER& hdr) {
	de.io_fixed_le(hdr.dwSize);
	de.io_fixed_le(hdr.dwHeaderFlags);
	de.io_fixed_le(hdr.dwHeight);
	de.io_fixed_le(hdr.dwWidth);
	de.io_fixed_le(hdr.dwPitchOrLinearSize);
	de.io_fixed_le(hdr.dwDepth);
	de.io_fixed_le(hdr.dwMipMapCount);
	de.advance(sizeof(hdr.dwReserved1[0]) * 11);

	de.io_fixed_le(hdr.ddspf.dwSize);
	de.io_fixed_le(hdr.ddspf.dwFlags);
	de.io_fixed_le(hdr.ddspf.dwFourCC);
	de.io_fixed_le(hdr.ddspf.dwRGBBitCount);
	de.io_fixed_le(hdr.ddspf.dwRBitMask);
	de.io_fixed_le(hdr.ddspf.dwGBitMask);
	de.io_fixed_le(hdr.ddspf.dwBBitMask);
	de.io_fixed_le(hdr.ddspf.dwABitMask);

	de.io_fixed_le(hdr.dwSurfaceFlags);
	de.io_fixed_le(hdr.dwCubemapFlags);
	de.advance(sizeof(hdr.dwReserved2[0]) * 3);

	if (hdr.dwSize != sizeof(hdr))
		throw SGE_ERROR("DDS invalid header size");

	if (hdr.ddspf.dwSize != sizeof(DDS_PIXELFORMAT))
		throw SGE_ERROR("DDS invalid header pixel format size");

	if (!(hdr.ddspf.dwFlags & DDS_FOURCC))
		throw SGE_ERROR("DDS invalid header pixel format");
}

void ImageIO_dds::Reader::_readHeader10(BinDeserializer& de, DDS_HEADER_DXT10& hdr10) {
	uint32_t tmp32;

	de.io_fixed_le(tmp32);
	hdr10.dxgiFormat = static_cast<DXGI_FORMAT>(tmp32);

	de.io_fixed_le(tmp32);
	hdr10.resourceDimension = static_cast<D3D10_RESOURCE_DIMENSION>(tmp32);

	de.io_fixed_le(hdr10.miscFlag);
	de.io_fixed_le(hdr10.arraySize);
	de.io_fixed_le(hdr10.miscFlags2);
}

void ImageIO_dds::Reader::load(Image& img, ByteSpan data, ColorType expectType) {
	BinDeserializer de(data);

	uint32_t sign;
	de.io_fixed_le(sign);

	if (sign != FourCC("DDS "))
		throw SGE_ERROR("Erros DDS file signature");

	DDS_HEADER hdr;
	_readHeader(de, hdr);

	auto colorType = ColorType::None;

	if (hdr.ddspf.dwFourCC == FourCC("DX10")) {
		DDS_HEADER_DXT10 hdr10;
		_readHeader10(de, hdr10);

		if (hdr10.resourceDimension != D3D10_RESOURCE_DIMENSION_TEXTURE2D)
			throw SGE_ERROR("Erros DDS texture dimension");

		switch (hdr10.dxgiFormat)
		{
			case DXGI_FORMAT_BC1_UNORM:  colorType = ColorType::BC1; break;
			case DXGI_FORMAT_BC2_UNORM:  colorType = ColorType::BC2; break;
			case DXGI_FORMAT_BC3_UNORM:  colorType = ColorType::BC3; break;
			case DXGI_FORMAT_BC4_UNORM:  colorType = ColorType::BC4; break;
			case DXGI_FORMAT_BC5_UNORM:  colorType = ColorType::BC5; break;
			case DXGI_FORMAT_BC7_UNORM:  colorType = ColorType::BC7; break;
			default: throw SGE_ERROR("Erros DDS10 file color format");
		}
	}else{
		switch (hdr.ddspf.dwFourCC) {
			case FourCC("DXT1"): colorType = ColorType::BC1; break;
			case FourCC("DXT2"): colorType = ColorType::BC2; break;
			case FourCC("DXT3"): colorType = ColorType::BC2; break;
			case FourCC("DXT4"): colorType = ColorType::BC3; break;
			case FourCC("DXT5"): colorType = ColorType::BC3; break;

			case FourCC("BC4U"): colorType = ColorType::BC4; break;
			case FourCC("BC5U"): colorType = ColorType::BC5; break;
			case FourCC("DXT7"): colorType = ColorType::BC7; break;
			default: throw SGE_ERROR("Erros DDS file color format");
		}
	}

	if (hdr.dwWidth % 4 || hdr.dwHeight % 4)
		throw SGE_ERROR("Erros DDS file width and height must be multiples of 4");

	int mipmapCount = 1;

	if (hdr.dwMipMapCount > 1) {
		uint32_t w = hdr.dwWidth;
		uint32_t h = hdr.dwHeight;
		for (uint32_t i = 0; i < hdr.dwMipMapCount; i++) {
			if (w & 0x3 || h & 0x3)
				break; // bypass non-multiple of 4 level

			w >>= 1;
			h >>= 1;
			mipmapCount = i + 1;
		}
	}

	int blockSize = ColorUtil::bytesPerPixelBlock(colorType);
	int strideInBytes = Math::max(1U, ((hdr.dwWidth + 3)/4)) * blockSize;

	img.create(colorType, hdr.dwWidth, hdr.dwHeight, strideInBytes, mipmapCount, de.remain());
	img.copyToPixelData(ByteSpan(de.cur(), de.remain()));
}

}