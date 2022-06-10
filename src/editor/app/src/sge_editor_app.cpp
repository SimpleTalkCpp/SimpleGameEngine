#include <sge_editor.h>

#include <sge_render/mesh/RenderMesh.h>
#include <sge_render/mesh/WavefrontObjLoader.h>
#include <sge_render/command/RenderCommand.h>
#include <sge_render/vertex/Vertex.h>
#include <sge_render/vertex/VertexLayoutManager.h>


namespace sge {

class MainWin : public NativeUIWindow {
	using Base = NativeUIWindow;
public:
	void onCreate(CreateDesc& desc) {
		SGE_DUMP_VAR(sizeof(Vertex_Pos));
		SGE_DUMP_VAR(sizeof(Vertex_PosColor));
//		SGE_DUMP_VAR(sizeof(Vertex_PosColorUv));
//		SGE_DUMP_VAR(sizeof(Vertex_PosColorUv2));

		Base::onCreate(desc);
		auto* renderer = Renderer::instance();

		{
			RenderContext::CreateDesc renderContextDesc;
			renderContextDesc.window = this;
			_renderContext = renderer->createContext(renderContextDesc);
		}

		_camera.setPos(0,2,2);
		_camera.setAim(0,0,0);

		auto shader = renderer->createShader("Assets/Shaders/test.shader");
		_material = renderer->createMaterial();
		_material->setShader(shader);

		EditMesh editMesh;

	#if 1
		WavefrontObjLoader::loadFile(editMesh, "Assets/Mesh/test.obj");
		// the current shader need color
		for (size_t i = editMesh.color.size(); i < editMesh.pos.size(); i++) {
			editMesh.color.emplace_back(255, 255, 255, 255);
		}

//		editMesh.normal.clear();
// 
		// the current shader has no uv
		editMesh.uv[0].clear();

	#else
		editMesh.pos.emplace_back( 0.0f,  0.5f, 0.0f);
		editMesh.pos.emplace_back( 0.5f, -0.5f, 0.0f);
		editMesh.pos.emplace_back(-0.5f, -0.5f, 0.0f);

		editMesh.color.emplace_back(255, 0, 0, 255);
		editMesh.color.emplace_back(0, 255, 0, 255);
		editMesh.color.emplace_back(0, 0, 255, 255);
	#endif

		_renderMesh.create(editMesh);

		VertexLayoutManager::instance()->getLayout(Vertex_Pos::kType);
	}

	virtual void onCloseButton() override {
		NativeUIApp::current()->quit(0);
	}

	virtual void onUIMouseEvent(UIMouseEvent& ev) override {
		if (ev.isDragging()) {
			using Button = UIMouseEventButton;
			switch (ev.pressedButtons) {
				case Button::Left: {
					auto d = ev.deltaPos * 0.01f;
					_camera.orbit(d.x, d.y);
				}break;

				case Button::Middle: {
					auto d = ev.deltaPos * 0.005f;
					_camera.move(d.x, d.y, 0);
				}break;

				case Button::Right: {
					auto d = ev.deltaPos * -0.005f;
					_camera.dolly(d.x + d.y);
				}break;
			}
		}
	}

	virtual void onDraw() {
		Base::onDraw();
		if (!_renderContext) return;

		_camera.setViewport(clientRect());

		{
			auto model	= Mat4f::s_identity();
			auto view	= _camera.viewMatrix();
			auto proj	= _camera.projMatrix();
			auto mvp	= proj * view;

			_material->setParam("sge_matrix_model", model);
			_material->setParam("sge_matrix_view",  view);
			_material->setParam("sge_matrix_proj",  proj);
			_material->setParam("sge_matrix_mvp",   mvp);

			_material->setParam("sge_camera_pos", _camera.pos());

			_material->setParam("sge_light_pos",	Vec3f(10, 10,   0));
			_material->setParam("sge_light_dir",	Vec3f(-5, -10, -2));
			_material->setParam("sge_light_power",	1.0f);
			_material->setParam("sge_light_color",	Vec3f(1,1,1));
		}

//-----
//		auto time = GetTickCount() * 0.001f;
//		auto s = abs(sin(time * 2));
		auto s = 1.0f;

		_material->setParam("test_float", s * 0.5f);
		_material->setParam("test_color", Color4f(s, 0, 0, 1));
//------

		_renderContext->setFrameBufferSize(clientRect().size);

		_renderContext->beginRender();

		_cmdBuf.reset();
		_cmdBuf.clearFrameBuffers()->setColor({0, 0, 0.2f, 1});
		_cmdBuf.drawMesh(SGE_LOC, _renderMesh, _material);
		_cmdBuf.swapBuffers();
		
		_renderContext->commit(_cmdBuf);

		_renderContext->endRender();
		drawNeeded();
	}

	SPtr<Material> _material;

	SPtr<RenderContext>	_renderContext;
	RenderCommandBuffer _cmdBuf;
	RenderMesh	_renderMesh;

	Math::Camera3f	_camera;
};

class EditorApp : public NativeUIApp {
	using Base = NativeUIApp;
public:
	virtual void onCreate(CreateDesc& desc) override {
		{
			String file = getExecutableFilename();
			String path = FilePath::dirname(file);
			path.append("/../../../../../../examples/Test101");

			auto* proj = ProjectSettings::instance();
			proj->setProjectRoot(path);
		}

		Base::onCreate(desc);

		Renderer::CreateDesc renderDesc;
		//renderDesc.apiType = OpenGL;
		Renderer::create(renderDesc);

	//---
		NativeUIWindow::CreateDesc winDesc;
		winDesc.isMainWindow = true;
		_mainWin.create(winDesc);
		_mainWin.setWindowTitle("SGE Editor");

	}

private:
	MainWin		_mainWin;
};

}

int main() {
	sge::EditorApp app;
	sge::EditorApp::CreateDesc desc;
	app.run(desc);

	return 0;
}
