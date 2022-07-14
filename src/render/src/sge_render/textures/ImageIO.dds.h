#pragma once

#include "Image.h"

namespace sge {

struct DDS_HEADER;
struct DDS_HEADER_DXT10;

struct ImageIO_dds {

	class Reader {
	public:
		void load(Image& img, ByteSpan data, ColorType expectType = ColorType::None);

	private:
		void _readHeader	(BinDeserializer& de, DDS_HEADER& hdr);
		void _readHeader10	(BinDeserializer& de, DDS_HEADER_DXT10& hdr10);
	};
};

} //namespace
