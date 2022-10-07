#include "RenderRequest.h"

namespace sge {

RenderRequest::RenderRequest() {
	light_pos   = Vec3f(10, 10,   0);
	light_dir   = Vec3f(-5, -10, -2);
	light_power = 4.0f;
	light_color = Vec3f(1,1,1);
}

void RenderRequest::reset(RenderContext* ctx) {
	_renderContext = ctx;
	commandBuffer.reset(ctx);
}

void RenderRequest::setMaterialCommonParams(Material* mtl) {
	if (!mtl) return;

	mtl->setParam("sge_matrix_model",	matrix_model);
	mtl->setParam("sge_matrix_view",	matrix_view);
	mtl->setParam("sge_matrix_proj",	matrix_proj);

	auto mvp = matrix_proj * matrix_view * matrix_model;
	mtl->setParam("sge_matrix_mvp",		mvp);

	mtl->setParam("sge_camera_pos",		camera_pos);

	mtl->setParam("sge_light_pos",		light_pos);
	mtl->setParam("sge_light_dir",		light_dir);
	mtl->setParam("sge_light_power",	light_power);
	mtl->setParam("sge_light_color",	light_color);
}


void RenderRequest::drawMesh(const SrcLoc& debugLoc, const RenderMesh& mesh, Material* material) {
	for (auto& sm : mesh.subMeshes()) {
		drawSubMesh(debugLoc, sm, material);
	}
}

void RenderRequest::drawSubMesh(const SrcLoc& debugLoc, const RenderSubMesh& subMesh, Material* material) {
	if (!material) { SGE_ASSERT(false); return; }

	setMaterialCommonParams(material);

	auto passes = material->passes();

	for (size_t i = 0; i < passes.size(); i++) {
		auto* cmd = addDrawCall();
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

}