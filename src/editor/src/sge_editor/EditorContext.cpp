#include "EditorContext.h"

namespace sge {

EditorContext* EditorContext::s_instance;

EditorContext* EditorContext::createContext() {
	SGE_ASSERT(s_instance == nullptr);
	s_instance = new EditorContext();
	return s_instance;
}

void EditorContext::destroyContext() {
	if (!s_instance) return;
	delete s_instance;
	s_instance = nullptr;
}

}

