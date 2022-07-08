#pragma once

#include "../base/Error.h"

namespace sge {

struct UtfUtil {
	UtfUtil() = delete;

	template<class DST, class SRC> static void convert(DST& dst, const SRC& src);

	template<size_t N>	static void append(String_<N>&  dst, StrView  view) { _append(dst, view); }
	template<size_t N>	static void append(String_<N>&  dst, StrViewW view) { _append(dst, view); }

	template<size_t N>	static void append(StringW_<N>& dst, StrView  view) { _append(dst, view); }
	template<size_t N>	static void append(StringW_<N>& dst, StrViewW view) { _append(dst, view); }

	template<class SRC>	static String  toString	(SRC& src) { String  o; convert(o, src); return o; }
	template<class SRC>	static StringW toStringW(SRC& src) { StringW o; convert(o, src); return o; }

private:
	static uint32_t _decodeUtf(const char*&     src, const char*     end);
	static uint32_t _decodeUtf(const char16_t*& src, const char16_t* end);
	static uint32_t _decodeUtf(const char32_t*& src, const char32_t* end);
	static uint32_t _decodeUtf(const wchar_t*&  src, const wchar_t*  end);

	template<class DST, class SRC> static void _append(DST& dst, SRC view);

	template<size_t N>
	static void _appendChar(String_<N>&  dst, uint32_t  ch) { _appendCharA(dst, ch); }
	static void _appendChar(String&      dst, uint32_t  ch) { _appendCharA(dst, ch); }

	template<size_t N>
	static void _appendChar(StringW_<N>& dst, uint32_t  ch) { _appendCharW(dst, ch); }
	static void _appendChar(StringW&     dst, uint32_t  ch) { _appendCharW(dst, ch); }

	template<class OUT_STR> static void _appendCharA(OUT_STR& dst, uint32_t  ch);
	template<class OUT_STR> static void _appendCharW(OUT_STR& dst, uint32_t  ch);
};

template<class DST, class SRC> inline
void UtfUtil::convert(DST& dst, const SRC& src) {
	dst.clear();
	append(dst, src);
}

template<class DST, class SRC> inline
void UtfUtil::_append(DST& dst, SRC view) {
	const auto* p = view.data();
	const auto* end = view.end();

	while (p < end) {
		auto v = _decodeUtf(p, end);
		_appendChar(dst, v);
	}
}

inline
uint32_t UtfUtil::_decodeUtf(const char* & src, const char* end ) {
	auto v = static_cast<uint8_t>(*src);
	uint32_t o = 0;
	if (v < 0x80) {
		o = static_cast<uint8_t>(*src); src++;
		return o;
	}

	if ((v & 0xE0) == 0xC0) {
		if( src+2 > end ) throw SGE_ERROR("");
		o += (static_cast<uint8_t>(*src) & 0x1FU) << 6; src++;
		o += (static_cast<uint8_t>(*src) & 0x3FU);      src++;
		return o;
	}

	if ((v & 0xF0) == 0xE0) {
		if( src+3 > end ) throw SGE_ERROR("");
		o += (static_cast<uint8_t>(*src) & 0x0FU) << 12; src++;
		o += (static_cast<uint8_t>(*src) & 0x3FU) << 6;  src++;
		o += (static_cast<uint8_t>(*src) & 0x3FU);       src++;
		return o;
	}

	if ((v & 0xF8) == 0xF0) {
		if( src+4 > end ) throw SGE_ERROR("");
		o += (static_cast<uint8_t>(*src) & 0x07U) << 18; src++;
		o += (static_cast<uint8_t>(*src) & 0x3FU) << 12; src++;
		o += (static_cast<uint8_t>(*src) & 0x3FU) << 6;  src++;
		o += (static_cast<uint8_t>(*src) & 0x3FU);       src++;
		return o;
	}

	if ((v & 0xFC) == 0xF8) {
		if( src+5 > end ) throw SGE_ERROR("");
		o += (static_cast<uint8_t>(*src) & 0x03U) << 24; src++;
		o += (static_cast<uint8_t>(*src) & 0x3FU) << 18; src++;
		o += (static_cast<uint8_t>(*src) & 0x3FU) << 12; src++;
		o += (static_cast<uint8_t>(*src) & 0x3FU) << 6;  src++;
		o += (static_cast<uint8_t>(*src) & 0x3FU);       src++;
	}

	if ((v & 0xFE) == 0xFC) {
		if( src+6 > end ) throw SGE_ERROR("");
		o += (static_cast<uint8_t>(*src) & 0x01U) << 30; src++;
		o += (static_cast<uint8_t>(*src) & 0x3FU) << 24; src++;
		o += (static_cast<uint8_t>(*src) & 0x3FU) << 18; src++;
		o += (static_cast<uint8_t>(*src) & 0x3FU) << 12; src++;
		o += (static_cast<uint8_t>(*src) & 0x3FU) << 6;  src++;
		o += (static_cast<uint8_t>(*src) & 0x3FU);       src++;
		return o;
	}

	{
		SGE_ASSERT(false);
		o = static_cast<uint8_t>(*src); src++;
		return o;
	}
}

inline
uint32_t UtfUtil::_decodeUtf(const char16_t*& src, const char16_t* end) {
	auto v = static_cast<uint16_t>(*src);

	if (v >= 0xD800 && v < 0xDBFF) {
		if( src+2 > end ) throw SGE_ERROR("");
		uint32_t a = static_cast<uint16_t>(*src); src++;
		uint32_t b = static_cast<uint16_t>(*src); src++;
		return ((a - 0xD800) << 10) | (b - 0xDC00);
	}

	uint32_t a = static_cast<uint16_t>(*src); src++;
	return a;
}

inline
uint32_t UtfUtil::_decodeUtf(const char32_t*& src, const char32_t* end) {
	return *src++;
}

inline
uint32_t UtfUtil::_decodeUtf(const wchar_t*& src, const wchar_t* end) {
	using C = WCharUtil::Char;
	const auto* & s = reinterpret_cast<const C* &>(src);
	const auto*   e = reinterpret_cast<const C*  >(end);
	return _decodeUtf(s, e);
}

template<class OUT_STR> inline
void UtfUtil::_appendCharA(OUT_STR& dst, uint32_t v) {
	if( v <       0x80 ) {
		dst += static_cast<char>(v);
		return;
	}

	if( v <    0x00800 ) {
		char t[] = {
			static_cast<char>(( v >> 6   ) | 0xC0),
			static_cast<char>(( v & 0x3F ) | 0x80),
			0
		};
		dst.append(t);
		return;
	}

	if( v <    0x10000 ) {
		char t[] = {
			static_cast<char>(( (v >> 12)        ) | 0xE0),
			static_cast<char>(( (v >> 6 ) & 0x3F ) | 0x80),
			static_cast<char>((  v        & 0x3F ) | 0x80),
			0
		};
		dst.append(t);
		return;
	}

	if( v <   0x200000 ) {
		char t[] = {
			static_cast<char>(( (v >> 18)        ) | 0xF0),
			static_cast<char>(( (v >> 12) & 0x3F ) | 0x80),
			static_cast<char>(( (v >> 6 ) & 0x3F ) | 0x80),
			static_cast<char>((  v        & 0x3F ) | 0x80),
			0
		};
		dst.append(t);
		return;
	}

// The patterns below are not part of UTF-8, but were part of the first specification.
	if( v <  0x4000000 ) {
		char t[] = {
			static_cast<char>(((v >> 24)        ) | 0xF8),
			static_cast<char>(((v >> 18) & 0x3F ) | 0x80),
			static_cast<char>(((v >> 12) & 0x3F ) | 0x80),
			static_cast<char>(((v >> 6 ) & 0x3F ) | 0x80),
			static_cast<char>(( v        & 0x3F ) | 0x80),
			0
		};
		dst.append(t);
		return;
	}

	if( v < 0x80000000 ) {
		char t[] = {
			static_cast<char>(((v >> 30)        ) | 0xFC),
			static_cast<char>(((v >> 24) & 0x3F ) | 0x80),
			static_cast<char>(((v >> 18) & 0x3F ) | 0x80),
			static_cast<char>(((v >> 12) & 0x3F ) | 0x80),
			static_cast<char>(((v >> 6 ) & 0x3F ) | 0x80),
			static_cast<char>(( v        & 0x3F ) | 0x80),
			0
		};
		dst.append(t);
		return;
	}

	{
		SGE_ASSERT(false);
		dst += static_cast<char>(v);
	}
}

template<class OUT_STR> inline
void UtfUtil::_appendCharW(OUT_STR& dst, uint32_t v) {
	if( v <  0x10000 ) {
		dst += static_cast<wchar_t>(v);
		return;
	}

	if( v < 0x110000 ) {
		wchar_t t[] = {
			static_cast<wchar_t>(( v >> 10   ) + 0xD800),
			static_cast<wchar_t>(( v & 0x3FF ) + 0xDC00),
			0
		};
		dst.append(t);
		return;
	}

	{
		SGE_ASSERT(false);
		dst += static_cast<wchar_t>(v);
		return;
	}
}

}