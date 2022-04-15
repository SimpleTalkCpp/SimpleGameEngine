#include "RenderContext.h"
#include "Renderer.h"

namespace sge {

RenderContext* RenderContext::create(CreateDesc& desc) {
	return Renderer::current()->onCreateContext(desc);
}

void RenderContext::render() {
	onBeginRender();
	onClearColorAndDepthBuffer();

	onTestDraw();

	onSwapBuffers();
	onEndRender();
}

RenderContext::RenderContext(CreateDesc& desc) {

}

}