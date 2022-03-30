#pragma once

#include "../base/NativeUIApp_Base.h"

#if SGE_OS_WINDOWS

namespace sge {

class NativeUIApp_Win32 : public NativeUIApp_Base {
	using Base = NativeUIApp_Base;
public:

	MSG _win32_msg;

protected:
	virtual void onCreate(CreateDesc& desc) override;
	virtual void onRun	() override;
	virtual	void onQuit	() override;
};

}

#endif