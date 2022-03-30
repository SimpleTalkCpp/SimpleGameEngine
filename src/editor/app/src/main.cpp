#include <sge_editor.h>

namespace sge {

class MainWin : public NativeUIWindow {
public:
	virtual void onCloseButton() override {
		NativeUIApp::current()->quit(0);
	}
};

class EditorApp : public NativeUIApp {
	using Base = NativeUIApp;
public:
	virtual void onCreate(CreateDesc& desc) override {
		Base::onCreate(desc);

		NativeUIWindow::CreateDesc winDesc;
		winDesc.isMainWindow = true;
		_mainWin.create(winDesc);
//		_mainWin.setWindowTitle("SGE Editor");
	}

private:
	MainWin _mainWin;
};

}

int main() {
	sge::EditorApp app;
	sge::EditorApp::CreateDesc desc;
	app.run(desc);

	return 0;
}
