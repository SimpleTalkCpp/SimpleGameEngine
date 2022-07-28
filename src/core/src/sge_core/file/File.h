#pragma once

#include "../string/UtfUtil.h"

namespace sge {

using FileSize = u64;

enum class FileMode {
	CreateNew,
	OpenExists,
	OpenOrCreate,
};

enum class FileAccess {
	Read,
	ReadWrite,
	WriteOnly,
};

enum class FileShareMode {
	None,
	Read,
	Write,
	ReadWrite,
};

struct File {
	static bool exists(StrView filename);
	static void rename(StrView src, StrView dst);

	static void	writeBytes(StrView filename, ByteSpan buf);
	static void writeText (StrView filename, StrView text);

	static char writeFile(StrView filename, ByteSpan data, bool createDir, bool logResult = true);
	static char writeFile(StrView filename, StrView  data, bool createDir, bool logResult = true);

						static void readFile (StrView filename, Vector<u8>&		outData) { _readFile(filename, outData); }
	template<size_t N>	static void readFile (StrView filename, Vector<u8, N>&	outData) { _readFile(filename, outData); }
	template<size_t N>	static void readFile (StrView filename, String_<N>&		outData) { _readFile(filename, outData); }

	static char writeFileIfChanged(	StrView filename, 
									ByteSpan data, 
									bool createDir, 
									bool logResult = true, 
									bool logNoChange = false);

	static char writeFileIfChanged(	StrView filename, 
									StrView data, 
									bool createDir, 
									bool logResult = true, 
									bool logNoChange = false);

private:
	template<class T> static void _readFile (StrView filename, T& outData);
};

template<class T> inline
void sge::File::_readFile(StrView filename, T& outData) {
	FileStream fs;
	fs.openRead(filename);
	auto size = fs.fileSize();
	if (size > eastl::numeric_limits<size_t>::max())
		throw SGE_ERROR("file is too large");
	outData.resize(size);

	Span<u8> span(reinterpret_cast<u8*>(outData.data()), outData.size());
	fs.readBytes(span);
}

}
