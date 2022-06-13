#pragma once

#if SGE_OS_WINDOWS

#include "Win32/NativeUIApp_Win32.h"
#include "Win32/NativeUIWindow_Win32.h"

namespace sge {
	using NativeUIWindow_Impl	= NativeUIWindow_Win32;
	using NativeUIApp_Impl		= NativeUIApp_Win32;
}

#else
	#error "unsupported platform"

#endif


namespace sge {

class NativeUIWindow : public NativeUIWindow_Impl {
};

class NativeUIApp : public NativeUIApp_Impl {
public:
	static NativeUIApp* instance() { return _instance; }
	NativeUIApp() { _instance = this; }

private:
	static NativeUIApp* _instance;
};

}