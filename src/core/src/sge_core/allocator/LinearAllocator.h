#pragma once

#include "../base/Error.h"

namespace sge {

class LinearAllocator {
public:
	void setChunkSize(size_t n) { _chunkSize = n; }

	void* allocate(size_t reqSize, size_t align = 16);

	void clear();

private:
	struct Chunk : public NonCopyable {
		Chunk(size_t n);

		void* allocate(size_t reqSize, size_t align);
		void clear();

	private:
		Vector<u8> _buffer;
		size_t _used = 0;
	};

	Vector<UPtr<Chunk>>	_chunks;
	size_t _chunkSize = 16 * 1024;
};


}