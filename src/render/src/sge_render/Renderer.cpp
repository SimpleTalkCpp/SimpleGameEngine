#include "Renderer.h"

#include "backend/dx11/Renderer_DX11.h"

namespace sge {

Renderer* Renderer::_current = nullptr;

Renderer::CreateDesc::CreateDesc() 
	: multithread(false)
{
#if SGE_OS_WINDOWS
	apiType = ApiType::DX11;
#else
	apiType = ApiType::None;
#endif
}

Renderer* Renderer::create(CreateDesc& desc) {
	Renderer* p = nullptr;
	switch (desc.apiType) {
		case ApiType::DX11: p = new Renderer_DX11(desc); break;
		default: throw SGE_ERROR("unsupport graphic api");
	}

	return p;
}

Renderer::Renderer() {
	SGE_ASSERT(_current == nullptr);
	_current = this;
	_vsync = true;
}

Renderer::~Renderer() {
	SGE_ASSERT(_current == this);
	_current = nullptr;
}

}
