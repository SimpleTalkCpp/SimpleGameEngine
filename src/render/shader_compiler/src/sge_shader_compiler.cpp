#include "ShaderCompiler_DX11.h"

namespace sge {

class ShaderCompiler : public ConsoleApp {
public:

protected:
	virtual void onRun() {
		{
			String file = getExecutableFilename();
			String path = FilePath::getDir(file);
			path.append("/../../../../../../examples/Test101");
			setCurrentDir(path);

			auto dir = getCurrentDir();
			SGE_LOG("dir = {}", dir);
		}		


		//ShaderCompiler_DX11 c;
		//c.compile(RenderShaderType::VertexShader, "");
	}

	void compile(StrView filename) {
	}

};

}

int main() {
	sge::ShaderCompiler app;
	app.run();

	return 0;
}