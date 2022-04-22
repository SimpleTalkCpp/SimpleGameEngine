#include "RenderCommand.h"
#include "mesh/RenderMesh.h"

namespace sge {

void RenderCommandBuffer::drawMesh(RenderMesh& mesh) {
	auto* cmd = newCommand<RenderCommand_DrawCall>();
	cmd->drawcall.vertexBuffer = mesh.vertexBuf();
}

void RenderCommandBuffer::swapBuffer() {
	auto* cmd = newCommand<RenderCommand_SwapBuffer>();
}

}