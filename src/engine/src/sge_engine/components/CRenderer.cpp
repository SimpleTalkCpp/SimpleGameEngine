#include "CRenderer.h"
#include "CRendererSystem.h"

namespace sge {

const TypeInfo* CRenderer::s_getType() {
	class TI : public TI_Base {
	public:
		TI() {
		}
	};
	static TI ti;
	return &ti;
}

CRenderer::CRenderer() {
	auto* sys = CRendererSystem::instance();
	sys->_internalAddComponent(this);
}

CRenderer::~CRenderer() {
	auto* sys = CRendererSystem::instance();
	sys->_internalRemoveComponent(this);
}

}