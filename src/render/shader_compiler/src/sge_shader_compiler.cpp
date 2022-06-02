#include "ShaderCompiler_DX11.h"
#include "ShaderParser.h"

namespace sge {

class ShaderCompiler : public ConsoleApp {
public:

protected:
	virtual void onRun() {
		{
			String file = getExecutableFilename();
			String path = FilePath::dirname(file);
			path.append("/../../../../../../examples/Test101");
			Directory::setCurrent(path);

			auto dir = Directory::getCurrent();
			SGE_LOG("dir = {}", dir);
		}		

		ShaderInfo info;

		StrView shaderFilename = "Assets/Shaders/test.shader";

		String outputPath = Fmt("LocalTemp/Imported/{}", shaderFilename);
		Directory::create(outputPath);

		{
			ShaderParser parser;
			parser.readFile(info, shaderFilename);
		}

		{ // DX11
			size_t passIndex = 0;
			for (auto& pass : info.passes) {
				auto passOutPath = Fmt("{}/dx11/pass{}", outputPath, passIndex);

				if (pass.vsFunc.size()) {
					ShaderCompiler_DX11 c;
					c.compile(passOutPath, ShaderStage::Vertex, shaderFilename, pass.vsFunc);
				}

				if (pass.psFunc.size()) {
					ShaderCompiler_DX11 c;
					c.compile(passOutPath, ShaderStage::Pixel, shaderFilename, pass.psFunc);
				}

				passIndex++;
			}
		}

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