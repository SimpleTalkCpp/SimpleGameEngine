#pragma once

#include "EditorWindow.h"

namespace sge {

class EditorHierarchyWindow : public EditorWindow {
public:

	void draw(RenderRequest& req, Scene& scene);

private:
	bool _active = true;
};

}