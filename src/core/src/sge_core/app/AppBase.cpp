#include "AppBase.h"

namespace sge {

#if SGE_OS_WINDOWS

void AppBase::setCurrentDir(StrView dir) {
	TempStringW tmp = UtfUtil::toStringW(dir);
	::SetCurrentDirectory(tmp.c_str());
}

String AppBase::getCurrentDir() {
	wchar_t tmp[MAX_PATH+1];
	if (!::GetCurrentDirectory(MAX_PATH, tmp))
		throw SGE_ERROR("getCurrentDir");
	String o = UtfUtil::toString(tmp);
	return o;
}

String AppBase::getExecutableFilename() {
	wchar_t tmp[MAX_PATH + 1];
	if (!::GetModuleFileName(nullptr, tmp, MAX_PATH))
		throw SGE_ERROR("");

	String o = UtfUtil::toString(tmp);
	return o;
}

#endif

}