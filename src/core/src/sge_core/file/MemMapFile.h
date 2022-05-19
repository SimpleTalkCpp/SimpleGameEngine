#pragma once

#include "FileStream.h"

namespace sge {

class MemMapFile : public NonCopyable {
public:
	~MemMapFile() { close(); }

	void open(StrView filename);
	void close();

	const u8* data() const { return _span.data(); }
	size_t    size() const { return _span.size(); }

	ByteSpan	span() const { return _span; }
	operator ByteSpan() { return _span; }

	const String& filename() const { return _fs.filename(); }

private:
	FileStream _fs;
#if SGE_OS_WINDOWS
	::HANDLE _mapping = nullptr; // !! it's null, not INVALID_HANDLE_VALUE for memory mapping 
#endif
	ByteSpan _span;
};

}
