#include "NativeUIApp_Base.h"

namespace sge {

int NativeUIApp_Base::run(CreateDesc& desc) {
	onCreate(desc);
	onRun();
	return _exitCode;
}

void NativeUIApp_Base::quit(int exitCode) {
	_exitCode = exitCode;
	onQuit();
}

}