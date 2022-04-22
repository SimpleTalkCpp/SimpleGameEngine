#pragma once

#include "../base/Error.h"

namespace sge {

class LinearAllocator {
public:

	void setChunkSize(size_t n) {
		_chunkSize = n;
	}

	void* allocate(size_t reqSize) {
		auto* c = getChunk(reqSize);
		return c->allocate(reqSize);
	}

	void clear() {
		for (auto& c : _chunks) {
			c->clear();
		}
	}

private:
	struct Chunk : public NonCopyable {
		Chunk(size_t n) {
			_buffer.resize(n);
			_available = n;
		}

		void* allocate(size_t reqSize) {
			if (_available < reqSize) {
				throw SGE_ERROR("not enough memory");
			}
			auto offset = _buffer.size() - _available;
			_available -= reqSize;
			return _buffer.data() + offset;
		}

		void clear() {
			_available = _buffer.size();
		}

		size_t available() const { return _available; }

	private:
		Vector<u8> _buffer;
		size_t _available = 0;
	};

	Chunk* getChunk(size_t reqSize) {
		if (_chunks.size()) {
			auto& t = _chunks.back();
			if (t->available() >= reqSize)
				return t.get();
		}

		auto chunkSize = reqSize > _chunkSize ? reqSize : _chunkSize;
		auto* newChunk = new Chunk(chunkSize);
		_chunks.emplace_back(newChunk);
		
		return newChunk;
	}

	Vector<UPtr<Chunk>>	_chunks;
	size_t _chunkSize = 4096;
};


}