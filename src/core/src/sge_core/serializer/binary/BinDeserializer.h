#pragma once

#include "BinSerializer.h"

namespace sge {

class BinDeserializer : public NonCopyable {
public:
	BinDeserializer(ByteSpan data) 
		: _data(data.data())
		, _cur(data.data())
		, _end(data.end())
	{}

	SGE_INLINE void io(i8 & value)	{ io_vary(value); }
	SGE_INLINE void io(i16& value)	{ io_vary(value); }
	SGE_INLINE void io(i32& value)	{ io_vary(value); }
	SGE_INLINE void io(i64& value)	{ io_vary(value); }

	SGE_INLINE void io(u8 & value)	{ io_vary(value); }
	SGE_INLINE void io(u16& value)	{ io_vary(value); }
	SGE_INLINE void io(u32& value)	{ io_vary(value); }
	SGE_INLINE void io(u64& value)	{ io_vary(value); }

	SGE_INLINE void io(f32& value)	{ io_fixed(value); }
	SGE_INLINE void io(f64& value)	{ io_fixed(value); }

	SGE_INLINE void io(char& value) { io_vary(value); }

	template<class T> SGE_INLINE
	void io(T& value) { BinSerializer_io(*this, value); }

	template<class T> SGE_INLINE
	BinDeserializer& operator<<(T& value) { io(value); return *this; }

//----------
	SGE_INLINE void io_fixed(i8 & value)	{ _io_fixed_le(value); }
	SGE_INLINE void io_fixed(i16& value)	{ _io_fixed_le(value); }
	SGE_INLINE void io_fixed(i32& value)	{ _io_fixed_le(value); }
	SGE_INLINE void io_fixed(i64& value)	{ _io_fixed_le(value); }

	SGE_INLINE void io_fixed(u8 & value)	{ _io_fixed_le(value); }
	SGE_INLINE void io_fixed(u16& value)	{ _io_fixed_le(value); }
	SGE_INLINE void io_fixed(u32& value)	{ _io_fixed_le(value); }
	SGE_INLINE void io_fixed(u64& value)	{ _io_fixed_le(value); }

	SGE_INLINE void io_fixed(f32& value)	{ _io_fixed_le(value); }
	SGE_INLINE void io_fixed(f64& value)	{ _io_fixed_le(value); }
//-----
	SGE_INLINE void io_fixed_le(i8 & value)	{ _io_fixed_le(value); }
	SGE_INLINE void io_fixed_le(i16& value)	{ _io_fixed_le(value); }
	SGE_INLINE void io_fixed_le(i32& value)	{ _io_fixed_le(value); }
	SGE_INLINE void io_fixed_le(i64& value)	{ _io_fixed_le(value); }
	SGE_INLINE void io_fixed_le(u8 & value)	{ _io_fixed_le(value); }
	SGE_INLINE void io_fixed_le(u16& value)	{ _io_fixed_le(value); }
	SGE_INLINE void io_fixed_le(u32& value)	{ _io_fixed_le(value); }
	SGE_INLINE void io_fixed_le(u64& value)	{ _io_fixed_le(value); }

	SGE_INLINE void io_fixed_le(f32& value)	{ _io_fixed_le(value); }
	SGE_INLINE void io_fixed_le(f64& value)	{ _io_fixed_le(value); }
//-----
	SGE_INLINE void io_fixed_be(i8 & value)	{ _io_fixed_be(value); }
	SGE_INLINE void io_fixed_be(i16& value)	{ _io_fixed_be(value); }
	SGE_INLINE void io_fixed_be(i32& value)	{ _io_fixed_be(value); }
	SGE_INLINE void io_fixed_be(i64& value)	{ _io_fixed_be(value); }

	SGE_INLINE void io_fixed_be(u8 & value)	{ _io_fixed_be(value); }
	SGE_INLINE void io_fixed_be(u16& value)	{ _io_fixed_be(value); }
	SGE_INLINE void io_fixed_be(u32& value)	{ _io_fixed_be(value); }
	SGE_INLINE void io_fixed_be(u64& value)	{ _io_fixed_be(value); }

	SGE_INLINE void io_fixed_be(f32& value)	{ _io_fixed_be(value); }
	SGE_INLINE void io_fixed_be(f64& value)	{ _io_fixed_be(value); }
//-----

	SGE_INLINE void io_vary(i8 & value)	{ _io_vary_signed<u8 >(value); }
	SGE_INLINE void io_vary(i16& value)	{ _io_vary_signed<u16>(value); }
	SGE_INLINE void io_vary(i32& value)	{ _io_vary_signed<u32>(value); }
	SGE_INLINE void io_vary(i64& value)	{ _io_vary_signed<u64>(value); }

	SGE_INLINE void io_vary(u8 & value)	{ _io_vary_unsigned(value); }
	SGE_INLINE void io_vary(u16& value)	{ _io_vary_unsigned(value); }
	SGE_INLINE void io_vary(u32& value)	{ _io_vary_unsigned(value); }
	SGE_INLINE void io_vary(u64& value)	{ _io_vary_unsigned(value); }

	SGE_INLINE void io_vary(char& value)	{ _io_vary_unsigned(reinterpret_cast<u8&>(value)); }

	SGE_INLINE const u8* cur() const { return _cur; }
	SGE_INLINE size_t remain() const { return _end - _cur; }
	SGE_INLINE const u8* advance(size_t n);

	void io_raw(u8* data, size_t dataSize) {
		auto* src = advance(dataSize);
		std::copy(src, src+dataSize, data);;
	}

private:
	template<class T> SGE_INLINE void _io_fixed_le(T& value);
	template<class T> SGE_INLINE void _io_fixed_be(T& value);
	template<class T> SGE_INLINE void _io_vary_unsigned(T& value);
	template<class U, class T> SGE_INLINE void _io_vary_signed(T& value);

	const u8* _data = nullptr;
	const u8* _cur  = nullptr;
	const u8* _end  = nullptr;
};

SGE_INLINE const u8* BinDeserializer::advance(size_t n) {
	if (remain() < n) {
		throw SGE_ERROR("BinDeserializer out of range");
	}
	const u8* old = _cur;
	_cur += n;
	return old;
}

template<class T> SGE_INLINE
void BinDeserializer::_io_fixed_le(T& value) {
	auto* p = advance(sizeof(value));
	value = LittleEndian::ToHost::get(*reinterpret_cast<const T*>(p));
}

template<class T> SGE_INLINE
void BinDeserializer::_io_fixed_be(T& value) {
	auto* p = advance(sizeof(value));
	value = BigEndian::ToHost::get(*reinterpret_cast<const T*>(p));
}

template<class T> SGE_INLINE
void BinDeserializer::_io_vary_unsigned(T& value) {
	size_t bit = 0;
	value = 0;
	for(size_t i = 0; i < sizeof(T) + 1; i++) {
		u8 t = *advance(1);
		value |= static_cast<T>( t & 0x7F ) << bit;
		if((t & 0x80) == 0) return;
		bit += 7;
		if( bit > sizeof(T) * 8 ) {
			throw SGE_ERROR("BinDeserializer out of range");
		}
	}
}

template<class U, class T> SGE_INLINE
void BinDeserializer::_io_vary_signed(T& value) {
// ZigZag encoding - https://developers.google.com/protocol-buffers/docs/encoding
	static_assert(sizeof(U) == sizeof(T));
	U tmp;
	_io_vary_unsigned(tmp);
	value = static_cast<T>(tmp >> 1) ^ -static_cast<T>(tmp & 1);
}

//===============================
template<> inline
void BinSerializer_io(BinDeserializer& se, String& v) {
	size_t len = 0;
	se.io(len);
	v.resize(len);
	try {
		se.io_raw(reinterpret_cast<u8*>(v.data()), len);
	} catch(...) {
		v.clear();
		throw;
	}
}

template<class T, size_t N, bool bEnableOverflow> inline
void BinSerializer_io(BinDeserializer& se, StringT<T, N, bEnableOverflow>& v) {
	size_t len = 0;
	se.io(len);
	v.resize(len);
	try {
		if (sizeof(T) == 1) {
			se.io_raw(reinterpret_cast<u8*>(v.data()), len);
		} else {
			for (size_t i = 0; i < len; i++) {
				se.io(v[i]);
			}
		}	
	} catch(...) {
		v.clear();
		throw;
	}
}

template<class T> inline
void BinSerializer_io(BinDeserializer& se, Vector<T>& v) {
	size_t len = 0;
	se.io(len);
	v.resize(len);
	try {
		for (size_t i = 0; i < len; i++) {
			se.io(v[i]);
		}
	} catch(...) {
		v.clear();
		throw;
	}
}

template<class T, size_t N, bool bEnableOverflow> inline
void BinSerializer_io(BinDeserializer& se, Vector<T, N, bEnableOverflow>& v) {
	size_t len = 0;
	se.io(len);
	v.resize(len);
	try {
		for (size_t i = 0; i < len; i++) {
			se.io(v[i]);
		}
	} catch(...) {
		v.clear();
		throw;
	}
}


} // namespace