#include "RenderRequest.h"

namespace sge {

RenderRequest::RenderRequest() {
	light_pos   = Vec3f(10, 10,   0);
	light_dir   = Vec3f(-5, -10, -2);
	light_power = 4.0f;
	light_color = Vec3f(1,1,1);
}

void RenderRequest::reset(RenderContext* ctx, Math::Camera3f& camera) {
	_renderContext = ctx;
	commandBuffer.reset(ctx);

	matrix_view   = camera.viewMatrix();
	matrix_proj   = camera.projMatrix();
	cameraPos     = camera.pos();
	cameraFrustum = camera.frustum();

	if (lineMaterial) {
		auto mvp = matrix_proj * matrix_view;
		lineMaterial->setParam("sge_matrix_mvp", mvp);
	}

	_inlineDraw.reset();
}

void RenderRequest::setMaterialCommonParams(Material* mtl, const Mat4f& matrix) {
	if (!mtl) return;

	mtl->setParam("sge_matrix_model",	matrix);
	mtl->setParam("sge_matrix_view",	matrix_view);
	mtl->setParam("sge_matrix_proj",	matrix_proj);

	auto mvp = matrix_proj * matrix_view * matrix;
	mtl->setParam("sge_matrix_mvp",		mvp);

	mtl->setParam("sge_camera_pos",		cameraPos);

	mtl->setParam("sge_light_pos",		light_pos);
	mtl->setParam("sge_light_dir",		light_dir);
	mtl->setParam("sge_light_power",	light_power);
	mtl->setParam("sge_light_color",	light_color);
}


void RenderRequest::drawMesh(const SrcLoc& debugLoc, const RenderMesh& mesh, Material* material, const Mat4f& matrix) {
	for (auto& sm : mesh.subMeshes()) {
		drawSubMesh(debugLoc, sm, material, matrix);
	}
}

void RenderRequest::drawSubMesh(const SrcLoc& debugLoc, const RenderSubMesh& subMesh, Material* material, const Mat4f& matrix) {
	if (!material) { SGE_ASSERT(false); return; }

	if (!cameraFrustum.isOverlapped(subMesh.boundingBox(), matrix)) {
		return;
	}

	setMaterialCommonParams(material, matrix);

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
		cmd->vertexOffset		= 0;
		cmd->vertexCount		= subMesh.vertexCount();
		cmd->indexBuffer		= subMesh.indexBuffer();
		cmd->indexType			= subMesh.indexType();
		cmd->indexOffset		= 0;
		cmd->indexCount			= subMesh.indexCount();
	}

	if (debug.drawBoundingBox) {
		drawBoundingBox(subMesh, Color4b(255,0,255,255), matrix);
	}
}

void RenderRequest::drawLines(Span<Vertex_PosColor> points, Span<u16> indices) {
	if (indices.size() <= 0) return;

	auto* cmd = addDrawCall();
	cmd->vertexOffset = _inlineDraw.vertexData.size();
	cmd->indexOffset  = _inlineDraw.indexData.size();

	_inlineDraw.vertexData.appendRange(spanCast<const u8>(points));
	 _inlineDraw.indexData.appendRange(spanCast<const u8>(indices));

	cmd->material  = lineMaterial;
	cmd->primitive = RenderPrimitiveType::Lines;
	cmd->vertexLayout = decltype(points)::element_type::s_layout();
	cmd->vertexBuffer = _inlineDraw.vertexBuffer;
	cmd->vertexCount  = 0;
	cmd->indexBuffer  = _inlineDraw.indexBuffer;
	cmd->indexType    = RenderDataType::UInt16;
	cmd->indexCount   = indices.size();
}

void RenderRequest::drawFrustum(const Math::Frustum3f& frustum, const Color4b& color) {
	Vertex_PosColor pt[8];

	for (size_t i = 0; i < 8; i++) {
		pt[i].pos = frustum.points[i];
		pt[i].color[0] = color;
	}

	u16 indices[] = {	0,1,
						1,2,
						2,3,
						3,0,

						4,5,
						5,6,
						6,7,
						7,4,
						
						0,4,
						1,5,
						2,6,
						3,7};

	drawLines(pt, indices);
}

void RenderRequest::drawBoundingBox(const BBox3f& bbox, const Color4b& color, const Mat4f& mat) {
	if (!bbox.isValid()) return;

	Vec3f pt[8];
	bbox.getPoints(pt, mat);

	Vertex_PosColor vertices[8];
	for (size_t i = 0; i < 8; i++) {
		auto& v = vertices[i];
		v.pos = pt[i];
		v.color[0] = color;
	}

	u16 indices[] = {	0,1,
						1,2,
						2,3,
						3,0,

						4,5,
						5,6,
						6,7,
						7,4,
						
						0,4,
						1,5,
						2,6,
						3,7};

	drawLines(vertices, indices);
}

void RenderRequest::drawBoundingBox(const RenderMesh& mesh, const Color4b& color, const Mat4f& mat) {
	for (auto& sm : mesh.subMeshes()) {
		drawBoundingBox(sm, color, mat);
	}
}

void RenderRequest::drawBoundingBox(const RenderSubMesh& mesh, const Color4b& color, const Mat4f& mat) {
	drawBoundingBox(mesh.boundingBox(), color, mat);
}

void RenderRequest::commit() {
	if (!_renderContext) return;

	_inlineDraw.uploadToGpu();
	_renderContext->commit(commandBuffer);
}

void RenderRequest::InlineDraw::reset() {
	vertexData.clear();
	indexData.clear();

	if (!vertexBuffer) {
		auto* renderer = Renderer::instance();
		RenderGpuBuffer::CreateDesc desc;
		desc.type = RenderGpuBufferType::Vertex;
		desc.bufferSize = 16;
		vertexBuffer = renderer->createGpuBuffer(desc);
	}

	if (!indexBuffer) {
		auto* renderer = Renderer::instance();
		RenderGpuBuffer::CreateDesc desc;
		desc.type = RenderGpuBufferType::Index;
		desc.bufferSize = 16;
		indexBuffer = renderer->createGpuBuffer(desc);
	}
}

void RenderRequest::InlineDraw::uploadToGpu() {
	_uploadToGpu(vertexBuffer, vertexData);
	_uploadToGpu(indexBuffer,  indexData );
}

void RenderRequest::InlineDraw::_uploadToGpu(SPtr<RenderGpuBuffer>& buf, const Vector<u8>& data) {
	auto n = data.size();

	if (n <= 0) return;
	if (buf->bufferSize() < n) {
		auto newSize = Math::nextPow2(n);
		auto desc = buf->desc();
		desc.bufferSize = newSize;
		buf->create(desc);
	}

	buf->uploadToGpu(data);
}

}