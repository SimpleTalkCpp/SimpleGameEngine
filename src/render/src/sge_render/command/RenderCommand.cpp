#include "RenderCommand.h"
#include "../mesh/RenderMesh.h"
#include "../mesh/RenderTerrain.h"

namespace sge {

void RenderCommandBuffer::reset() {
	for (auto* cmd : _commands) {
		cmd->~RenderCommand();
	}
	_commands.clear();
	_allocator.clear();
}

}