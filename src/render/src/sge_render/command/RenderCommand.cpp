#include "RenderCommand.h"
#include "../mesh/RenderMesh.h"

namespace sge {

void RenderCommandBuffer::drawMesh(RenderMesh& mesh) {
	auto* cmd = newCommand<RenderCommand_DrawCall>();
	cmd->primitive		= mesh.primitive();
	cmd->vertexLayout	= mesh.vertexLayout();
	cmd->vertexBuffer	= mesh.vertexBuf();
	cmd->vertexCount	= mesh.vertexCount();
}

void RenderCommandBuffer::reset() {
	_allocator.clear();
	for (auto* cmd : _commands) {
		cmd->~RenderCommand();
	}
	_commands.clear();
}

}