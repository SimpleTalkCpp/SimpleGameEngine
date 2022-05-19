#include <sge_core/log/Log.h>

#include "Directory.h"
#include "File.h"
#include "FilePath.h"
#include "FileStream.h"
#include "MemMapFile.h"

namespace sge {

void File::writeBytes(StrView filename, ByteSpan buf) {
	FileStream	s;
	s.openWrite(filename, true );
	s.writeBytes(buf);
}

void File::writeText(StrView filename, StrView text) {
	writeBytes(filename, ByteSpan_make(text));
}

char File::writeFile(StrView filename, StrView data, bool createDir, bool logResult /*= true*/) {
	auto byteSpan = ByteSpan_make(data);
	return writeFile(filename, byteSpan, createDir, logResult);
}

char File::writeFile(StrView filename, ByteSpan data, bool createDir, bool logResult /*= true*/) {
	char op = '+';

	auto realPath = FilePath::realpath(filename);

	if (File::exists(realPath)) {
		op = 'U';
	}

	if (logResult) {
		SGE_LOG("[{}] {}, size={}", op, realPath, data.size());
	}

	if (createDir) {
		auto dir = FilePath::dirname(realPath);
		if (dir.size()) {
			Directory::create(dir);
		}
	}

	writeBytes(realPath, data);
	return op;
}

char File::writeFileIfChanged(StrView filename, StrView data, bool createDir, bool logResult /*= true*/, bool logNoChange /*= false*/) {
	auto byteSpan = ByteSpan_make(data);
	return writeFileIfChanged(filename, byteSpan, createDir, logResult, logNoChange);
}

char File::writeFileIfChanged(StrView filename, ByteSpan data, bool createDir, bool logResult /*= true*/, bool logNoChange /*= false*/) {
	char op = '+';

	auto realPath = FilePath::realpath(filename);

	if (exists(realPath)) {
		MemMapFile map;		
		map.open(realPath);

		if (map.span() == data) {
			op = '=';
		}else{
			op = 'U';
		}
	}

	if (logResult) {
		if (op != '=' || logNoChange) {
			SGE_LOG("[{}] {}, size={}", op, realPath, data.size());
		}
	}

	if (op == '=') return op;
	return writeFile(realPath, data, createDir, false);
}


#if SGE_OS_WINDOWS
#if 0
#pragma mark ================= Windows ====================
#endif

bool File::exists(StrView filename) {
	TempStringW filenameW;
	UtfUtil::convert(filenameW, filename);
	DWORD dwAttrib = ::GetFileAttributes(filenameW.c_str());
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void File::rename(StrView src, StrView dst) {
	TempStringW srcW, dstW;
	UtfUtil::convert(srcW, src);
	UtfUtil::convert(dstW, dst);

	if( 0 != ::_wrename(srcW.c_str(), dstW.c_str() ) ) {
		throw SGE_ERROR("rename file {}->{}", src, dst);
	}
}

#else
#if 0
#pragma mark ================= UNIX ====================
#endif

bool File::exists(StrView filename ) {
	TempStringA filenameA;
	UtfUtil::convert(filenameA, filename);
	struct stat s;
	if( 0 != ::stat( filenameA.c_str(), &s ) ) return false;
	return ( s.st_mode & S_IFDIR ) == 0;
}

void File::rename(StrView src, StrView dst) {
	TempStringA srcA, dstA;
	UtfUtil::convert(srcA, src);
	UtfUtil::convert(dstA, dst);

	auto ret = ::rename(srcA.c_str(), dstA.c_str() );
	if( ret != 0 ) {
		throw SGE_ERROR("rename file {}->{}", src, dst);
	}
}

#endif

}