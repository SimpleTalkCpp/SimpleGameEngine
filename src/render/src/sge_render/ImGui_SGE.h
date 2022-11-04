#pragma once

#include <imgui.h>
#include <sge_render/textures/Texture.h>
#include <sge_render/shader/Material.h>

namespace sge {

class RenderContext;
class RenderRequest;

class ImGui_SGE : public NonCopyable {
public:
	~ImGui_SGE();

	void create();
	void destroy();

	void onBeginRender(RenderContext* renderContext);
	void onEndRender(RenderContext* renderContext);

	void onDrawUI(RenderRequest& req);
	bool onUIMouseEvent(UIMouseEvent& ev);

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

inline Vec2f Vec2f_make(const ImVec2& v) { return Vec2f(v.x, v.y); }
inline Vec4f Vec4f_make(const ImVec4& v) { return Vec4f(v.x, v.y, v.z, v.w); }

}