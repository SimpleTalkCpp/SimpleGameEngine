#pragma once

#include "command/RenderCommand.h"

namespace sge {

class RenderCommandBuffer;

struct RenderContext_CreateDesc {
	NativeUIWindow*	window = nullptr;
};

class RenderContext : public Object {
public:
	using CreateDesc = RenderContext_CreateDesc;

	void beginRender() { onBeginRender(); }
	void endRender()   { onEndRender();   }

	void setFrameBufferSize(Vec2f newSize);

	void commit(RenderCommandBuffer& cmdBuf) { onCommit(cmdBuf); }

	RenderContext(CreateDesc& desc);
	virtual ~RenderContext() = default;

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
				CMD_CASE(SwapBuffers);
				CMD_CASE(DrawCall);
				default:
					throw SGE_ERROR("unhandled command");
			}
		}

		#undef CMD_CASE
	}
};

}