#include "RenderGpuBuffer.h"

namespace sge {
	
void RenderGpuBuffer::uploadToGpu(ByteSpan data, size_t offset) {
	if (data.size() + offset > _desc.bufferSize) {
		throw SGE_ERROR("out of range");
	}
	onUploadToGpu(data, offset);
}

void RenderGpuBuffer::create(CreateDesc& desc) {
	_desc = desc;
	onCreate(desc);
}

}