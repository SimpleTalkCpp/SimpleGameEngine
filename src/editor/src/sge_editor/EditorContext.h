#pragma once

#include "EntitySelection.h"

namespace sge {

class EditorContext : public NonCopyable {
public:
	static EditorContext*	instance() { return s_instance; }

	static EditorContext* createContext();
	static void destroyContext();

	EntitySelection		entitySelection;

private:
	static EditorContext*	s_instance;
};

}