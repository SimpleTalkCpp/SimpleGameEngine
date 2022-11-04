#pragma once

#include "Component.h"

namespace sge {

class CRendererSystem;

class CRenderer : public Component {
	SGE_ABSTRACT_OBJECT_TYPE(CRenderer, Component)
public:

	CRenderer();
	~CRenderer();

friend class CRendererSystem;
protected:
	void _render(RenderRequest& req) { onRender(req); }
	virtual void onRender(RenderRequest& req) = 0;
};

}