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
	static void write(StrView filename, Span<const u8> data);
	static void read (StrView filename, Vector<u8> data);
};

}
