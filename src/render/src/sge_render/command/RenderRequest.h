#pragma once

#include "RenderCommand.h"
#include <sge_render/mesh/RenderMesh.h>

namespace sge {

class RenderRequest : public Object {
public:	
	Mat4f		matrix_model;
	Mat4f		matrix_view;
	Mat4f		matrix_proj;

	Vec3f		camera_pos;

	Vec3f		light_pos;
	Vec3f		light_dir;
	float		light_power;
	Vec3f		light_color;

	RenderCommandBuffer	commandBuffer;

	RenderRequest();

	void reset(RenderContext* ctx);

	//TODO: move to separate cbuffer
	void setMaterialCommonParams(Material* mtl);

	void drawMesh	(const SrcLoc& debugLoc, const RenderMesh&    mesh,	   Material* material);
	void drawSubMesh(const SrcLoc& debugLoc, const RenderSubMesh& subMesh, Material* material);

	RenderCommand_ClearFrameBuffers*	clearFrameBuffers() { return commandBuffer.clearFrameBuffers(); }
	RenderCommand_SwapBuffers*			swapBuffers()		{ return commandBuffer.swapBuffers(); }
	RenderCommand_DrawCall*				addDrawCall()		{ return commandBuffer.addDrawCall(); }

	SGE_NODISCARD	RenderScissorRectScope	scissorRectScope()	{ return RenderScissorRectScope(&commandBuffer); }
	SGE_INLINE		void setScissorRect(const Rect2f& rect) { commandBuffer.setScissorRect(rect); }

private:
	RenderContext*	_renderContext = nullptr;
};

}