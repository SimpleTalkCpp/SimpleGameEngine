#include "RenderContext.h"
#include "Renderer.h"

namespace sge {

void RenderContext::setFrameBufferSize(Vec2f newSize) {
	if (_frameBufferSize == newSize)
		return;

	_frameBufferSize = newSize;
	onSetFrameBufferSize(newSize);
}

RenderContext::RenderContext(CreateDesc& desc) {
}

}