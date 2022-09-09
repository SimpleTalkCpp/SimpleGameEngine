#pragma once

#include <imgui.h>
#include <sge_render/textures/Texture.h>
#include <sge_render/shader/Material.h>

namespace sge {

class RenderRequest;

class ImGui_SGE : public NonCopyable {
public:
	~ImGui_SGE();

	void create();
	void destroy();

	void onBeginRender(RenderContext* renderContext);
	void onEndRender(RenderContext* renderContext);

	void onDrawUI(RenderRequest& req);
	void onUIMouseEvent(UIMouseEvent& ev);

	using Vertex = Vertex_ImGui;

private:
	void _createFontTexture();
	int _mouseButton(UIMouseEventButton v);

	SPtr<Shader>	_shader;
	SPtr<Material>	_material;
	SPtr<Texture2D>	_fontTex;

	const VertexLayout*		_vertexLayout = nullptr;

	SPtr<RenderGpuBuffer>	_vertexBuffer;
	SPtr<RenderGpuBuffer>	_indexBuffer;

	Vector<u8>	_vertexData;
	Vector<u8>	_indexData;

	ImGuiContext*	_ctx = nullptr;
};

}