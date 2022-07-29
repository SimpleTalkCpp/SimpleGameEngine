#include "Directory.h"

#include <sge_core/string/StringUtil.h>
#include <sge_core/base/Error.h>
#include "FilePath.h"

namespace sge {

void Directory::create(StrView path) {
	if (exists(path)) return;

	auto parent = FilePath::dirname(path);
	if (parent.size()) {
		create(parent); // create recursively
	}

	_create(path);
}


#if SGE_OS_WINDOWS
#if 0
#pragma mark ================= Windows ====================
#endif

void Directory::setCurrent(StrView dir) {
	TempStringW tmp = UtfUtil::toStringW(dir);
	::SetCurrentDirectory(tmp.c_str());
}

String Directory::getCurrent() {
	wchar_t tmp[MAX_PATH+1];
	if (!::GetCurrentDirectory(MAX_PATH, tmp))
		throw SGE_ERROR("GetCurrentDirectory");
	String o = UtfUtil::toString(tmp);
	return o;
}

void Directory::_create(StrView path) {
	TempStringW pathW;
	UtfUtil::convert(pathW, path);
	auto ret = ::CreateDirectory(pathW.c_str(), nullptr);
	if (!ret) throw SGE_ERROR("create directory {}", pathW);
}

bool Directory::exists(StrView path) {
	TempStringW pathW;
	UtfUtil::convert(pathW, path);

	DWORD dwAttrib = ::GetFileAttributes(pathW.c_str());
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

#else

#if 0
#pragma mark ================= Unix ====================
#endif

Directory::_create(StrView path) {
	TempStringA pathA;
	UtfUtil::convert(pathA, path);
	auto ret = ::mkdir(pathA.c_str(), 0755);
	if (ret != 0) throw SGE_ERROR("create directory {}", pathA);
}

bool Directory::exists(StrView path ) {
	TempStringA pathA;
	UtfUtil::convert(pathA, path);

	struct stat s;
	if( 0 != ::stat( pathA.c_str(), &s ) ) return false;
	return ( s.st_mode & S_IFDIR ) != 0;
}

#endif

}