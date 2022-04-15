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

String NativeUIApp_Win32::onGetExecutableFilename() {
	wchar_t tmp[MAX_PATH + 1];
	if (!::GetModuleFileName(nullptr, tmp, MAX_PATH))
		throw SGE_ERROR("");

	String o = UtfUtil::toString(tmp);
	return o;
}

String NativeUIApp_Win32::onGetCurrentDir() {
	wchar_t tmp[MAX_PATH+1];
	if (!::GetCurrentDirectory(MAX_PATH, tmp))
		throw SGE_ERROR("getCurrentDir");
	String o = UtfUtil::toString(tmp);
	return o;
}

void NativeUIApp_Win32::onSetCurrentDir(StrView dir) {
	TempStringW tmp = UtfUtil::toStringW(dir);
	::SetCurrentDirectory(tmp.c_str());
}

}

#endif
