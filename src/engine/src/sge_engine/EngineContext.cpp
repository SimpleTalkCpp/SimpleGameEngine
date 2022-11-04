#include "EngineContext.h"

#include <sge_engine/components/CTransform.h>
#include <sge_engine/components/CMeshRenderer.h>
#include <sge_engine/components/CRendererSystem.h>

namespace sge {

EngineContext* EngineContext::s_instance = nullptr;

EngineContext* EngineContext::create() {
	SGE_ASSERT(s_instance == nullptr);
	s_instance = new EngineContext();
	return s_instance;
}

void EngineContext::destroy() {
	if (s_instance) {
		delete s_instance;
		s_instance = nullptr;
	}
}

EngineContext::EngineContext() {
	registerComponentType<CTransform>();
//	registerComponentType<CRenderer>();
//	registerComponentType<CMeshRenderer>();

	registerSystem<CRendererSystem>();
}

void EngineContext::_registerComponentType(const TypeInfo* type) {
	_componentTypeMap.insert(type->name, type);
}

void EngineContext::_registerSystem(CSystemBase* sys) {
	_systems.emplace_back(sys);
}

}