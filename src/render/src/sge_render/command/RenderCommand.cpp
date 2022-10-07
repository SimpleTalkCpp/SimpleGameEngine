#include "RenderCommand.h"
#include "../mesh/RenderMesh.h"
#include "../mesh/RenderTerrain.h"

namespace sge {

void RenderCommandBuffer::reset(RenderContext* ctx) {
	_renderContext = ctx;

	for (auto* cmd : _commands) {
		cmd->~RenderCommand();
	}
	_commands.clear();
	_allocator.clear();

	_scissorRect = Rect2f(Vec2f(0,0), ctx->frameBufferSize());
}

}