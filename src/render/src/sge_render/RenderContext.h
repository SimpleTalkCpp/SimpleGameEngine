#pragma once

namespace sge {

struct RenderContext_CreateDesc {
	NativeUIWindow*	window = nullptr;
};

class RenderContext : public NonCopyable {
public:
	using CreateDesc = RenderContext_CreateDesc;

	static RenderContext*	create(CreateDesc& desc);

	void render();

	RenderContext(CreateDesc& desc);
	virtual ~RenderContext() = default;

protected:
	virtual void onBeginRender() {};
	virtual void onEndRender() {};
	virtual void onClearColorAndDepthBuffer() {}
	virtual void onSwapBuffers() {}
};


}