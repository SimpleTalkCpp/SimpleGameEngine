#pragma once

#include "command/RenderCommand.h"
#include "ImGui_SGE.h"

namespace sge {

class RenderCommandBuffer;
class RenderRequest;

struct RenderContext_CreateDesc {
	NativeUIWindow*	window = nullptr;
};

class RenderContext : public Object {
public:
	using CreateDesc = RenderContext_CreateDesc;

	void beginRender();
	void endRender();

	void setFrameBufferSize(Vec2f newSize);
	const Vec2f& frameBufferSize() const { return _frameBufferSize; }

	void commit(RenderCommandBuffer& cmdBuf) { onCommit(cmdBuf); }

	RenderContext(CreateDesc& desc);
	virtual ~RenderContext() = default;

	void onPostCreate();
	void drawUI(RenderRequest& req);
	bool onUIMouseEvent(UIMouseEvent& ev);

protected:

	virtual void onBeginRender() {};
	virtual void onEndRender() {};
	virtual void onSetFrameBufferSize(Vec2f newSize) {};
	virtual void onCommit(RenderCommandBuffer& cmdBuf) {}

	Vec2f	_frameBufferSize {0,0};

	template<class IMPL>
	void _dispatch(IMPL* impl, RenderCommandBuffer& cmdBuf) {
		using Cmd = RenderCommandType;

		#define CMD_CASE(E) \
			case Cmd::E: { \
				auto* c = static_cast<RenderCommand_##E*>(cmd); \
				impl->onCmd_##E(*c); \
			} break; \
		//----

		for (auto* cmd : cmdBuf.commands()) {
			switch (cmd->type()) {
				CMD_CASE(ClearFrameBuffers)
				CMD_CASE(SwapBuffers)
				CMD_CASE(DrawCall)
				CMD_CASE(SetScissorRect)
				default:
					throw SGE_ERROR("unhandled command");
			}
		}

		#undef CMD_CASE
	}

	ImGui_SGE _imgui;
};

}