#include <sge_editor.h>
#include <sge_render/mesh/RenderTerrain.h>

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
		auto* editor = EditorContext::instance();

		{
			RenderContext::CreateDesc renderContextDesc;
			renderContextDesc.window = this;
			_renderContext = renderer->createContext(renderContextDesc);
		}

//		_camera.setPos(0,5,5);
		_camera.setPos(0,10,10);
		_camera.setAim(0,0,0);

		{
			Texture2D_CreateDesc texDesc;
			auto& image = texDesc.imageToUpload;

			#if 1
				// image.loadFile("Assets/Textures/uvChecker.png");
				// image.loadFile("Assets/Textures/uvChecker_BC1.dds");
				// image.loadFile("Assets/Textures/uvChecker_BC2.dds");
				// image.loadFile("Assets/Textures/uvChecker_BC3.dds");
				image.loadFile("Assets/Textures/uvChecker_BC7.dds");

				texDesc.size = image.size();
				texDesc.colorType = image.colorType();

			#else
				int w = 256;
				int h = 256;

				texDesc.size.set(w, h);
				texDesc.colorType = ColorType::RGBAb;

				image.create(Color4b::kColorType, w, h);

				for (int y = 0; y < w; y++) {
					auto span = image.row<Color4b>(y);
					for (int x = 0; x < h; x++) {
						span[x] = Color4b(	static_cast<u8>(x),
											static_cast<u8>(y), 
											0, 
											255);
					}
				}
			#endif

			_testTexture = renderer->createTexture2D(texDesc);
		}

		{
			auto shader = renderer->createShader("Assets/Shaders/test.shader");
			_material = renderer->createMaterial();
			_material->setShader(shader);

			_material->setParam("mainTex", _testTexture);

			EditMesh editMesh;

		#if 1
			WavefrontObjLoader::loadFile(editMesh, "Assets/Mesh/test.obj");
			// the current shader need color
			for (size_t i = editMesh.color.size(); i < editMesh.pos.size(); i++) {
				editMesh.color.emplace_back(255, 255, 255, 255);
			}

		#else
			editMesh.pos.emplace_back( 0.0f,  0.5f, 0.0f);
			editMesh.pos.emplace_back( 0.5f, -0.5f, 0.0f);
			editMesh.pos.emplace_back(-0.5f, -0.5f, 0.0f);

			editMesh.color.emplace_back(255, 0, 0, 255);
			editMesh.color.emplace_back(0, 255, 0, 255);
			editMesh.color.emplace_back(0, 0, 255, 255);
		#endif

			_renderMesh.create(editMesh);
		}

		{
			float size = 2048;
			float pos  = size / -2;
			float y    = -100;
			float height = 200;
			int maxLod = 6;
			_terrain.createFromHeightMapFile(
				Vec3f(pos, y, pos),
				Vec2f(size, size),
				height, 
				maxLod, 
				"Assets/Terrain/TerrainTest/TerrainHeight_Small.png");
		}

		{ // ECS
			Vector<Entity*> entities;

			for (int i = 0; i < 100; i++) {
				auto* e = _scene.addEntity("Entity");
				auto* t = e->transform();
				t->setLocalPos(static_cast<float>(i), 5, 10);
				entities.emplace_back(e);

				if (i > 5) {
					entities[i / 5]->transform()->addChild(e->transform());
				}
			}

//			editor->entitySelection.add(EntityId(1));
			editor->entitySelection.add(EntityId(3));
		}
	}

	virtual void onCloseButton() override {
		NativeUIApp::instance()->quit(0);
	}

	virtual void onUIMouseEvent(UIMouseEvent& ev) override {
		_renderContext->onUIMouseEvent(ev);

		if (ev.isDragging()) {
			using Button = UIMouseEventButton;
			switch (ev.pressedButtons) {
				case Button::Left: {
					auto d = ev.deltaPos * 0.01f;
					_camera.orbit(d.x, d.y);
				}break;

				case Button::Middle: {
					auto d = ev.deltaPos * 0.1f;
					_camera.move(d.x, d.y, 0);
				}break;

				case Button::Right: {
					auto d = ev.deltaPos * -0.1f;
					_camera.dolly(d.x + d.y);
				}break;
			}
		}
	}

	virtual void onDraw() {
		Base::onDraw();
		if (!_renderContext) return;

		_camera.setViewport(clientRect());

		_renderContext->setFrameBufferSize(clientRect().size);
		_renderContext->beginRender();

		_renderRequest.reset(_renderContext);
		_renderRequest.matrix_model = Mat4f::s_identity();
		_renderRequest.matrix_view  = _camera.viewMatrix();
		_renderRequest.matrix_proj  = _camera.projMatrix();
		_renderRequest.camera_pos   = _camera.pos();

		_renderRequest.clearFrameBuffers()->setColor({0, 0, 0.2f, 1});

//-----
//		auto time = GetTickCount() * 0.001f;
//		auto s = abs(sin(time * 2));
		auto s = 1.0f;

		_material->setParam("test_float", s * 0.5f);
		_material->setParam("test_color", Color4f(s, s, s, 1));
//------
		_renderRequest.drawMesh(SGE_LOC, _renderMesh, _material);

//		_terrain.render(_renderRequest);

		_hierarchyWindow.draw(_renderRequest, _scene);
		_inspectorWindow.draw(_renderRequest, _scene);

//		ImGui::ShowDemoWindow(nullptr);

		_renderContext->drawUI(_renderRequest);
		_renderRequest.swapBuffers();
		_renderContext->commit(_renderRequest.commandBuffer);

		_renderContext->endRender();
		drawNeeded();
	}

	SPtr<Material>		_material;
	SPtr<Texture2D>		_testTexture;

	SPtr<RenderContext>	_renderContext;
	RenderMesh			_renderMesh;

	RenderTerrain		_terrain;

	Math::Camera3f		_camera;
	Scene				_scene;

	RenderRequest		_renderRequest;

	EditorHierarchyWindow		_hierarchyWindow;
	EditorInspectorWindow		_inspectorWindow;
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

	#if 1 // compile shaders for quick testing
		{
			SHELLEXECUTEINFO ShExecInfo = {0};
			ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
			ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
			ShExecInfo.hwnd = NULL;
			ShExecInfo.lpVerb = L"open";
			ShExecInfo.lpFile = L"compile_shaders.bat";
			ShExecInfo.lpParameters = L"";
			ShExecInfo.lpDirectory = NULL;
			ShExecInfo.nShow = SW_SHOW;
			ShExecInfo.hInstApp = NULL; 
			ShellExecuteEx(&ShExecInfo);
			WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
			CloseHandle(ShExecInfo.hProcess);
		}
	#endif

		Base::onCreate(desc);

		Renderer::CreateDesc renderDesc;
		//renderDesc.apiType = OpenGL;
		Renderer::create(renderDesc);

		EditorContext::createContext();

	//---
		NativeUIWindow::CreateDesc winDesc;
		winDesc.isMainWindow = true;
		_mainWin.create(winDesc);
		_mainWin.setWindowTitle("SGE Editor");

	}

	virtual void onQuit() {
		EditorContext::destroyContext();
		EngineContext::destroy();
		Base::onQuit();
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
