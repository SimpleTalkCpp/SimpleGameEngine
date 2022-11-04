#include "ImGui_SGE.h"
#include <sge_render/Renderer.h>
#include <sge_render/RenderContext.h>
#include <sge_render/command/RenderRequest.h>

namespace sge {

ImGui_SGE::~ImGui_SGE() {
	destroy();
}

void ImGui_SGE::create() {
	if (!IMGUI_CHECKVERSION()) throw SGE_ERROR("ImGui version error");
	_ctx = ImGui::CreateContext();
	if (!_ctx) throw SGE_ERROR("ImGui error create context");

	_vertexLayout = Vertex::s_layout();

	ImGuiIO& io = ImGui::GetIO();
	io.BackendRendererUserData = this;
	io.BackendRendererName = "ImGui_SGE";
	io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
	io.ConfigFlags  |= ImGuiConfigFlags_NavEnableKeyboard;

	auto* renderer = Renderer::instance();
	_shader   = renderer->createShader("Assets/Shaders/imgui.shader");
	_material = renderer->createMaterial();
	_material->setShader(_shader);

}

void ImGui_SGE::destroy() {
	if (!_ctx) return;

	ImGuiIO& io = ImGui::GetIO();
	io.BackendRendererUserData = nullptr;
	io.BackendRendererName = nullptr;

	ImGui::DestroyContext(_ctx);
	_ctx = nullptr;
}

void ImGui_SGE::onBeginRender(RenderContext* renderContext) {
	ImGuiIO& io = ImGui::GetIO();

	auto s = renderContext->frameBufferSize();

	io.DisplaySize = ImVec2(s.x, s.y);
	io.DeltaTime = 1.0f / 60.0f;

	if (!_fontTex) {
		_createFontTexture();
	}

	ImGui::NewFrame();
}

void ImGui_SGE::onEndRender(RenderContext* renderContext) {
}

void ImGui_SGE::onDrawUI(RenderRequest& req) {
	ImGui::Render();

	if (!_material) return;

	auto* data = ImGui::GetDrawData();
	if (!data) return;

	if (data->DisplaySize.x <= 0 || data->DisplaySize.y <= 0)
		return;

	if (data->TotalVtxCount <= 0 || data->TotalIdxCount <= 0)
		return;

	auto* renderer = Renderer::instance();

	{
		float L = data->DisplayPos.x;
		float R = data->DisplayPos.x + data->DisplaySize.x;
		float T = data->DisplayPos.y;
		float B = data->DisplayPos.y + data->DisplaySize.y;

		Mat4f mat(
			{ 2.0f/(R-L),   0.0f,           0.0f,       0.0f },
			{ 0.0f,         2.0f/(T-B),     0.0f,       0.0f },
			{ 0.0f,         0.0f,           0.5f,       0.0f },
			{ (R+L)/(L-R),  (T+B)/(B-T),    0.5f,       1.0f }
		);

		_material->setParam("ProjectionMatrix", mat);
		_material->setParam("texture0", _fontTex);
	}

	auto vertexSize = sizeof(ImDrawVert);
	auto indexSize  = sizeof(ImDrawIdx);

	SGE_ASSERT(vertexSize == _vertexLayout->stride);
	SGE_ASSERT(indexSize == sizeof(u16));

	auto totalVertexDataSize = data->TotalVtxCount * vertexSize;
	auto totalIndexDataSize  = data->TotalIdxCount * indexSize;

	if (!_vertexBuffer || _vertexBuffer->bufferSize() < totalVertexDataSize) {
		RenderGpuBuffer_CreateDesc desc;
		desc.type = RenderGpuBufferType::Vertex;
		desc.bufferSize = totalVertexDataSize;
		_vertexBuffer = renderer->createGpuBuffer(desc);
	}

	if (!_indexBuffer || _indexBuffer->bufferSize() < totalIndexDataSize) {
		RenderGpuBuffer_CreateDesc desc;
		desc.type = RenderGpuBufferType::Index;
		desc.bufferSize = totalIndexDataSize;
		_indexBuffer = renderer->createGpuBuffer(desc);
	}

	auto scissorRectScope = req.scissorRectScope();

	{
		_vertexData.clear();
		_indexData.clear();

		int vertexStart = 0;
		int indexStart  = 0;

		for (int i = 0; i < data->CmdListsCount; i++) {
			auto* srcCmd = data->CmdLists[i];

			for (int j = 0; j < srcCmd->CmdBuffer.Size; j++) {
				auto& srcBuf = srcCmd->CmdBuffer[j];

				// Project scissor/clipping rectangles into frame buffer space

				ImVec2 clip_off = data->DisplayPos;
				ImVec2 clip_min(srcBuf.ClipRect.x - clip_off.x, srcBuf.ClipRect.y - clip_off.y);
				ImVec2 clip_max(srcBuf.ClipRect.z - clip_off.x, srcBuf.ClipRect.w - clip_off.y);

				if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
					continue;

				// Apply scissor/clipping rectangle
				auto a = Vec2f_make(clip_min);
				auto b = Vec2f_make(clip_max);

				req.setScissorRect({a, b - a});

				auto* cmd = req.commandBuffer.addDrawCall();

				#if _DEBUG
					cmd->debugLoc = SGE_LOC;
				#endif

				cmd->material			= _material;
				cmd->materialPassIndex	= 0;
				cmd->primitive			= RenderPrimitiveType::Triangles;

				cmd->vertexLayout		= _vertexLayout;
				cmd->vertexBuffer		= _vertexBuffer;
				cmd->vertexOffset		= (vertexStart + srcBuf.VtxOffset) * vertexSize;
				cmd->vertexCount		= 0;

				cmd->indexType			= RenderDataTypeUtil::get<ImDrawIdx>();
				cmd->indexBuffer		= _indexBuffer;
				cmd->indexOffset		= (indexStart + srcBuf.IdxOffset) * indexSize;
				cmd->indexCount			= srcBuf.ElemCount;
			}

			vertexStart += srcCmd->VtxBuffer.Size;
			indexStart  += srcCmd->IdxBuffer.Size;

			_vertexData.appendRange(Span<const u8>(reinterpret_cast<const u8*>(srcCmd->VtxBuffer.Data), srcCmd->VtxBuffer.Size * vertexSize));
			 _indexData.appendRange(Span<const u8>(reinterpret_cast<const u8*>(srcCmd->IdxBuffer.Data), srcCmd->IdxBuffer.Size * indexSize ));
		}

		_vertexBuffer->uploadToGpu(_vertexData);
		 _indexBuffer->uploadToGpu(_indexData);
	}
}

bool ImGui_SGE::onUIMouseEvent(UIMouseEvent& ev) {
	ImGuiIO& io = ImGui::GetIO();
	
	using Type = UIMouseEventType;
	switch (ev.type) {
		case Type::Move: {
			io.AddMousePosEvent(ev.pos.x, ev.pos.y);
		} break;

		case Type::Down: {
			io.AddMouseButtonEvent(_mouseButton(ev.pressedButtons), true);
		} break;

		case Type::Up: {
			io.AddMouseButtonEvent(_mouseButton(ev.pressedButtons), false);
		} break;

		case Type::Scroll: {
			io.AddMouseWheelEvent(ev.scroll.x, ev.scroll.y);
		} break;
	}

	return io.WantCaptureMouse;
}

void ImGui_SGE::_createFontTexture() {
	ImGuiIO& io = ImGui::GetIO();

	unsigned char* pixels = nullptr;
	int width  = 0;
	int height = 0;
	io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);

	using Color = ColorRb;

	Texture2D_CreateDesc desc;
	desc.size.set(width, height);
	desc.colorType = Color::kColorType;
	desc.imageToUpload.create(Color::kColorType, width, height);
	desc.imageToUpload.copyToPixelData(ByteSpan(pixels, width * height));

	auto* renderer = Renderer::instance();
	_fontTex = renderer->createTexture2D(desc);
}

int ImGui_SGE::_mouseButton(UIMouseEventButton v) {
	using Button = UIMouseEventButton;
	switch (v) {
		case Button::Left:		return 0;
		case Button::Right:		return 1;
		case Button::Middle:	return 2;
		case Button::Button4:	return 3;
		case Button::Button5:	return 4;
	}
	return 0;
}

}