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

	static void readFile (StrView filename, Vector<u8> data);

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
};

}
