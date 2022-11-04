#pragma once

#include "CSystemBase.h"

namespace sge {

class CRendererSystem : public CSystemBase {
public:
	static CRendererSystem* instance() { return _instance; }

	CRendererSystem();
	~CRendererSystem();

	void _internalAddComponent(CRenderer* c);
	void _internalRemoveComponent(CRenderer* c);

	void render(RenderRequest& req);

private:
	static CRendererSystem*	_instance;
	Vector<CRenderer*>		_rendererComponents;
};

}