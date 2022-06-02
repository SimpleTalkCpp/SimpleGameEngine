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

		auto shader = renderer->createShader("Assets/Shaders/test.shader");
		_material = renderer->createMaterial();
		_material->setShader(shader);
		//_material.setParam("a", 10.0f);

		EditMesh editMesh;

	#if 1
		WavefrontObjLoader::loadFile(editMesh, "Assets/Mesh/test.obj");
		// the current shader need color
		for (size_t i = editMesh.color.size(); i < editMesh.pos.size(); i++) {
			editMesh.color.emplace_back(255, 255, 255, 255);
		}

		// the current shader has no uv or normal
		editMesh.uv[0].clear();
		editMesh.normal.clear();

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

	virtual void onDraw() {
		Base::onDraw();
		if (!_renderContext) return;

		_renderContext->setFrameBufferSize(clientRect().size);

		_renderContext->beginRender();

		_cmdBuf.reset();
		_cmdBuf.clearFrameBuffers()->setColor({0, 0, 0.2f, 1});
		_cmdBuf.drawMesh(SGE_LOC, _renderMesh);//, _material);
		_cmdBuf.swapBuffers();
		
		_renderContext->commit(_cmdBuf);

		_renderContext->endRender();
		drawNeeded();
	}

	SPtr<Material> _material;

	SPtr<RenderContext>	_renderContext;
	RenderCommandBuffer _cmdBuf;
	RenderMesh	_renderMesh;
};

class EditorApp : public NativeUIApp {
	using Base = NativeUIApp;
public:
	virtual void onCreate(CreateDesc& desc) override {
		{
			String file = getExecutableFilename();
			String path = FilePath::dirname(file);
			path.append("/../../../../../../examples/Test101");
			Directory::setCurrent(path);

			auto dir = Directory::getCurrent();
			SGE_LOG("dir = {}", dir);
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
