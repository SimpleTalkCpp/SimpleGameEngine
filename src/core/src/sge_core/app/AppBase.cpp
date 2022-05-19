#include "AppBase.h"

namespace sge {

#if SGE_OS_WINDOWS

String AppBase::getExecutableFilename() {
	wchar_t tmp[MAX_PATH + 1];
	if (!::GetModuleFileName(nullptr, tmp, MAX_PATH))
		throw SGE_ERROR("");

	String o = UtfUtil::toString(tmp);
	return o;
}

#endif

}