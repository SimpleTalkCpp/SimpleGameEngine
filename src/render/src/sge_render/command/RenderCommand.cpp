#include "RenderCommand.h"
#include "../mesh/RenderMesh.h"

namespace sge {

void RenderCommandBuffer::drawMesh(const SrcLoc& debugLoc, const RenderMesh& mesh, Material* material) {
	for (auto& sm : mesh.subMeshes()) {
		drawSubMesh(debugLoc, sm, material);
	}
}

void RenderCommandBuffer::drawSubMesh(const SrcLoc& debugLoc, const RenderSubMesh& subMesh, Material* material) {
	if (!material) { SGE_ASSERT(false); return; }

	auto passes = material->passes();

	for (size_t i = 0; i < passes.size(); i++) {
		auto* cmd = newCommand<RenderCommand_DrawCall>();
		#if _DEBUG
			cmd->debugLoc = debugLoc;
		#endif

		cmd->material			= material;
		cmd->materialPassIndex	= i;

		cmd->primitive			= subMesh.primitive();
		cmd->vertexLayout		= subMesh.vertexLayout();
		cmd->vertexBuffer		= subMesh.vertexBuffer();
		cmd->vertexCount		= subMesh.vertexCount();
		cmd->indexBuffer		= subMesh.indexBuffer();
		cmd->indexType			= subMesh.indexType();
		cmd->indexCount			= subMesh.indexCount();
	}
}

void RenderCommandBuffer::reset() {
	for (auto* cmd : _commands) {
		cmd->~RenderCommand();
	}
	_commands.clear();
	_allocator.clear();
}

}