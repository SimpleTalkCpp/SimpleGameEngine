#include "Texture.h"

namespace sge {

Texture2D::Texture2D(CreateDesc& desc) {
	_type			= DataType::Texture2D;
	_colorType		= desc.colorType;
	_mipmapCount	= desc.mipmapCount;
	_size			= desc.size;
}

} // namespace