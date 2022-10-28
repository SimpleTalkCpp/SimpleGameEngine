#include "EditorContext.h"

namespace sge {

EditorContext* EditorContext::s_instance;

EditorContext* EditorContext::createContext() {
	EngineContext::create();

	SGE_ASSERT(s_instance == nullptr);
	s_instance = new EditorContext();
	return s_instance;
}

void EditorContext::destroyContext() {
	if (!s_instance) return;
	delete s_instance;
	s_instance = nullptr;
}

EditorContext::EditorContext() {

#define E(T) { \
		static EditorPropertyDrawer_##T d; \
		registerPropertyDrawer(TypeOf<T>(), &d); \
	} \
//---
	E(float)
	E(Vec3f)
#undef E
}

void EditorContext::registerPropertyDrawer(const TypeInfo* type, EditorPropertyDrawer* drawer) {
	_propertyDrawerMap[type] = drawer;
}

EditorPropertyDrawer* EditorContext::getPropertyDrawer(const TypeInfo* type) {
	auto it = _propertyDrawerMap.find(type);
	if (it == _propertyDrawerMap.end()) {
		if (type->fields().size()) {
			auto* structDrawer = &_defaultStructPropertyDrawerMap[type];
			registerPropertyDrawer(type, structDrawer);
			return structDrawer;
		}
		return nullptr;
	}
	return it->second;
}

}

