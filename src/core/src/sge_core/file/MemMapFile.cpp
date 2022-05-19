#include "MemMapFile.h"

namespace sge {

#if SGE_OS_WINDOWS
	
void MemMapFile::open(StrView filename) {
	close();
	_fs.openRead(filename);

	if (_fs.fileSize() >= SIZE_T_MAX)
		throw SGE_ERROR("memmap file size too larget");

	auto size = static_cast<size_t>(_fs.fileSize());
	if (size <= 0) return;

	_mapping = ::CreateFileMapping(_fs.nativeFd(), nullptr, PAGE_READONLY, 0, 0, nullptr);
	if (!_mapping) {
		throw SGE_ERROR("memmap");
	}

	auto* data = reinterpret_cast<u8*>(::MapViewOfFile(_mapping, FILE_MAP_READ, 0, 0, 0));
	if (!data) {
		throw SGE_ERROR("memmap");
	}

	_span = ByteSpan(data, size);
}

void MemMapFile::close() {
	if (_span.size() <= 0) return;
	::UnmapViewOfFile(_span.data());

	if (_mapping) {
		::CloseHandle(_mapping);
		_mapping = nullptr;
	}

	_span = ByteSpan();
	_fs.close();
}

#else

void MemMapFile::open(StrView filename) {
	close();
	_fs.openRead(filename);

	if (_fs.fileSize() >= SIZE_T_MAX)
		throw SGE_ERROR("memmap file size too larget");

	auto size = static_cast<size_t>(_fs.fileSize());
	if (size <= 0) return;

	auto* data = reinterpret_cast<u8*>(::mmap(0, size, PROT_READ, MAP_PRIVATE, _fs.nativeFd(), 0));
	if (data == MAP_FAILED || data == nullptr) {
		throw SGE_ERROR("memmap");
	}
	_span = ByteSpan(data, size);
}

void MemMapFile::close() {
	if (_span.data()) {	
		::munmap(_span.data(), _span.size());
	}
	_span = ByteSpan();
	_fs.close();
}

#endif

}