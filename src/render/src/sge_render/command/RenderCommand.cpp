#include "RenderCommand.h"
#include "../mesh/RenderMesh.h"

namespace sge {

void RenderCommandBuffer::drawMesh(const SrcLoc& debugLoc, const RenderMesh& mesh) {
	for (auto& sm : mesh.subMeshes()) {
		drawSubMesh(debugLoc, sm);
	}
}

void RenderCommandBuffer::drawSubMesh(const SrcLoc& debugLoc, const RenderSubMesh& subMesh) {
	auto* cmd = newCommand<RenderCommand_DrawCall>();
#if _DEBUG
	cmd->debugLoc = debugLoc;
#endif

	cmd->primitive		= subMesh.primitive();
	cmd->vertexLayout	= subMesh.vertexLayout();
	cmd->vertexBuffer	= subMesh.vertexBuffer();
	cmd->vertexCount	= subMesh.vertexCount();
	cmd->indexBuffer	= subMesh.indexBuffer();
	cmd->indexType		= subMesh.indexType();
	cmd->indexCount		= subMesh.indexCount();
}

void RenderCommandBuffer::reset() {
	_allocator.clear();
	for (auto* cmd : _commands) {
		cmd->~RenderCommand();
	}
	_commands.clear();
}

}