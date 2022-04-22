#include "RenderContext.h"
#include "Renderer.h"
#include "RenderCommand.h"

namespace sge {

//void RenderContext::render() {
//	onBeginRender();
//	onClearColorAndDepthBuffer();
//
//	onTestDraw();
//
//	onSwapBuffers();
//	onEndRender();
//}

void RenderContext::render(RenderCommandBuffer& cmdBuf) {
	using Cmd = RenderCommandType;

	for (auto* cmd : cmdBuf.commands) {
		switch (cmd->type()) {
			case Cmd::DrawCall: {
				auto* c = static_cast<RenderCommand_DrawCall*>(cmd);
				
			} break;
		}
	}
}

RenderContext::RenderContext(CreateDesc& desc) {

}

}