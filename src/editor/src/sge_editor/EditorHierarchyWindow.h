#pragma once

#include "EditorWindow.h"

namespace sge {

class EditorHierarchyWindow : public EditorWindow {
public:

	void draw(Scene& scene, RenderRequest& req);

private:
	bool _active = true;
};

}