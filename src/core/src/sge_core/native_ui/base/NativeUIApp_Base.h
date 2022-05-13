#pragma once

#include <sge_core/app/AppBase.h>

namespace sge {

class NativeUIApp_Base : public AppBase {
public:
	class CreateDesc {
	};

			int  run(CreateDesc& desc);
			void quit(int returnCode);
	virtual void willQuit() {}

protected:
	virtual void onCreate(CreateDesc& desc) {}
	virtual void onRun() {}
	virtual	void onQuit() {}

	int _exitCode = 0;
};

}