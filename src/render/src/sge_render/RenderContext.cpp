#include "RenderContext.h"
#include "Renderer.h"

namespace sge {

RenderContext* RenderContext::create(CreateDesc& desc) {
	return Renderer::current()->onCreateContext(desc);
}

RenderContext::RenderContext(CreateDesc& desc) {

}

}