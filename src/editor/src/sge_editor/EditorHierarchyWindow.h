#pragma once

#include "EditorWindow.h"

namespace sge {

class EditorHierarchyWindow : public EditorWindow {
public:

	void draw(RenderRequest& req, Scene& scene);

private:
	struct DrawRequest {
		RenderRequest&		req;
		EntitySelection&	sel;
	};

	void drawEntity(DrawRequest& dr, CTransform* tran);
	void drawEntityChildren(DrawRequest& dr, CTransform* tran);

	bool _active = true;
};

}