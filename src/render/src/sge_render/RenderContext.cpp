#include "RenderContext.h"
#include "Renderer.h"

namespace sge {

void RenderContext::beginRender() {
	_imgui.onBeginRender(this);
	onBeginRender();
}

void RenderContext::endRender() {
	onEndRender();
	_imgui.onEndRender(this);
}

void RenderContext::setFrameBufferSize(Vec2f newSize) {
	if (_frameBufferSize == newSize)
		return;

	_frameBufferSize = newSize;
	onSetFrameBufferSize(newSize);
}

RenderContext::RenderContext(CreateDesc& desc) {
}

void RenderContext::onPostCreate() {
	_imgui.create();
}

void RenderContext::drawUI(RenderRequest& req) {
	_imgui.onDrawUI(req);
}

void RenderContext::onUIMouseEvent(UIMouseEvent& ev) {
	_imgui.onUIMouseEvent(ev);
}

}