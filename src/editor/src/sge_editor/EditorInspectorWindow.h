#pragma once

#include "EditorWindow.h"

namespace sge {

class EditorInspectorWindow : public EditorWindow {
public:

	void draw(Scene& scene, RenderRequest& req);

private:
	bool _active = true;
};

}