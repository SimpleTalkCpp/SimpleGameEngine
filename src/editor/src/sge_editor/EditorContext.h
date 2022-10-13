#pragma once

#include "EntitySelection.h"
#include "EditorPropertyDrawer.h"

namespace sge {

class EditorContext : public NonCopyable {
public:
	static EditorContext*	instance() { return s_instance; }

	static EditorContext* createContext();
	static void destroyContext();

	EditorContext();

	void registerPropertyDrawer(const TypeInfo* type, EditorPropertyDrawer* drawer);

	EditorPropertyDrawer* getPropertyDrawer(const TypeInfo* type);

	EntitySelection		entitySelection;

private:
	Map<const TypeInfo*, EditorPropertyDrawer_struct>	_defaultStructPropertyDrawerMap;
	Map<const TypeInfo*, EditorPropertyDrawer*>			_propertyDrawerMap;

	static EditorContext*	s_instance;
};

}