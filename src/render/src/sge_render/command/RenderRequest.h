#pragma once

#include "RenderCommand.h"

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

	void reset();

	//TODO: move to separate cbuffer
	void setMaterialCommonParams(Material* mtl);

	void drawMesh	(const SrcLoc& debugLoc, const RenderMesh&    mesh,	   Material* material);
	void drawSubMesh(const SrcLoc& debugLoc, const RenderSubMesh& subMesh, Material* material);

	RenderCommand_ClearFrameBuffers* clearFrameBuffers() {
		return commandBuffer.newCommand<RenderCommand_ClearFrameBuffers>();
	}

	RenderCommand_SwapBuffers* swapBuffers() {
		return commandBuffer.newCommand<RenderCommand_SwapBuffers>();
	}

};

}