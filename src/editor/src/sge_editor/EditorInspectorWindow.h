#pragma once

#include "EditorWindow.h"

namespace sge {

class EditorInspectorWindow : public EditorWindow {
public:

	void draw(RenderRequest& req, Scene& scene);

private:

	using ObjectList = Vector<Object*, 32>;
	Map<const TypeInfo*, ObjectList>	_tempTypeMap;
	Vector<ObjectList*>					_tempTypeList;

	void _addToTempList(Component* c);
	void drawComponent(RenderRequest& req, ObjectList& list);

	bool _active = true;
};

}