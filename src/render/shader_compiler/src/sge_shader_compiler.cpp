#include "ShaderCompiler_DX11.h"
#include "ShaderParser.h"

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

		ShaderInfo info;

		ShaderParser parser;
		parser.readFile(info, "Assets/Shaders/test.shader");

		//ShaderCompiler_DX11 c;
		//c.compile(RenderShaderType::VertexShader, "");

		SGE_LOG("---- end ----");
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