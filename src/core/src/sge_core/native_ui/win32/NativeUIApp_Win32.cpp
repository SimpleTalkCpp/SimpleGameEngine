#include "NativeUIApp_Win32.h"
#include "../NativeUI.h"
#include "../../string/UtfUtil.h"

#if SGE_OS_WINDOWS

namespace sge {

void NativeUIApp_Win32::onCreate(CreateDesc& desc) {
	Base::onCreate(desc);
}

void NativeUIApp_Win32::onRun() {
	Base::onRun();

	while (GetMessage(&_win32_msg, NULL, 0, 0)) {
		TranslateMessage(&_win32_msg);
		DispatchMessage(&_win32_msg);
	}

	willQuit();
}

void NativeUIApp_Win32::onQuit() {
	Base::onQuit();

	::PostQuitMessage(_exitCode);
}

}

#endif
