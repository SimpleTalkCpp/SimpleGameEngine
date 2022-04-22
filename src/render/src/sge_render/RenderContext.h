#pragma once

namespace sge {

class RenderCommandBuffer;

struct RenderContext_CreateDesc {
	NativeUIWindow*	window = nullptr;
};

class RenderContext : public Object {
public:
	using CreateDesc = RenderContext_CreateDesc;

	void render(RenderCommandBuffer& cmdBuf);

	RenderContext(CreateDesc& desc);
	virtual ~RenderContext() = default;

protected:
	virtual void onBeginRender() {};
	virtual void onTestDraw() {};
	virtual void onEndRender() {};
	virtual void onClearColorAndDepthBuffer() {}
	virtual void onSwapBuffers() {}
};


}