#pragma once

#include "sge_core-config.h"

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif

#include "../detect_platform/sge_detect_platform.h"

#if SGE_OS_WINDOWS
	#define NOMINMAX 1
	#include <WinSock2.h> // WinSock2.h must include before windows.h to avoid winsock1 define
	#include <ws2tcpip.h> // struct sockaddr_in6
	#pragma comment(lib, "Ws2_32.lib")
	#include <Windows.h>
	#include <intsafe.h>
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netdb.h>
	#include <netinet/in.h> // struct sockaddr_in
#endif

#include <cassert>
#include <exception>
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <atomic>

#include <EASTL/vector.h>
#include <EASTL/fixed_vector.h>
#include <EASTL/string.h>
#include <EASTL/fixed_string.h>
#include <EASTL/string_view.h>
#include <EASTL/span.h>

#include <EASTL/optional.h>

#include <EASTL/map.h>
#include <EASTL/hash_map.h>
#include <EASTL/vector_map.h>
#include <EASTL/string_map.h>

#include <EASTL/set.h>

#include <EASTL/unique_ptr.h>
#include <EASTL/shared_ptr.h>
#include <EASTL/weak_ptr.h>

#include <nlohmann/json.hpp>

#include "sge_macro.h"

using Json = nlohmann::json;

//==== EASTL ====

#if !EASTL_DLL // If building a regular library and not building EASTL as a DLL...
	// It is expected that the application define the following
	// versions of operator new for the application. Either that or the
	// user needs to override the implementation of the allocator class.
inline void* operator new[](size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line) { return malloc(size); }
inline void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line) {
#if SGE_OS_WINDOWS
	return _aligned_malloc(size, alignment);
#else
	return std::aligned_alloc(alignment, size);
#endif
}
#endif

//===============

namespace sge {

template<class T> inline constexpr typename std::underlying_type<T>::type     enumInt(T  value) { return       static_cast<typename std::underlying_type<T>::type>(value); }
template<class T> inline constexpr typename std::underlying_type<T>::type& enumIntRef(T& value) { return *reinterpret_cast<typename std::underlying_type<T>::type*>(&value); }
template<class T> inline constexpr typename std::underlying_type<T>::type const& enumIntRef(const T& value) { return *reinterpret_cast<const typename std::underlying_type<T>::type*>(&value); }

template<class T> inline bool constexpr enumHas(const T& a, const T& b) { return static_cast<T>(enumInt(a) & enumInt(b)) != static_cast<T>(0); }

template<class T> SGE_INLINE T* constCast(const T* v) { return const_cast<T*>(v); }
template<class T> SGE_INLINE T& constCast(const T& v) { return const_cast<T&>(v); }

template<class T> SGE_INLINE void swap(T& a, T& b) { T tmp = std::move(a); a = std::move(b); b = std::move(tmp); }

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

// using f16 = half;
using f32 = float;
using f64 = double;
using f128 = long double;

template< class Obj, class Member > constexpr
intptr_t memberOffset(Member Obj::*ptrToMember) {
	Obj* c = nullptr;
	Member* m = &(c->*ptrToMember);
	return reinterpret_cast<intptr_t>(m);
}

template<class T> using UPtr = eastl::unique_ptr<T>;

template<class T> using Span = eastl::span<T>;
using ByteSpan = Span<const u8>;

template<class DST, class SRC> inline 
Span<DST> spanCast(Span<SRC> src) {
	size_t sizeInBytes = src.size() * sizeof(SRC);
	return Span<DST>(reinterpret_cast<DST*>(src.data()), sizeInBytes / sizeof(DST));
}

template<class T, size_t N, bool bEnableOverflow = true>
struct Vector_Base {
	using Type = typename eastl::fixed_vector<T, N, bEnableOverflow>;
};

template<class T>
struct Vector_Base<T, 0, true> {
	using Type = typename eastl::vector<T>;
};

template<class T, size_t N = 0, bool bEnableOverflow = true>
class Vector : public Vector_Base<T, N, bEnableOverflow>::Type {
	using Base = typename Vector_Base<T, N, bEnableOverflow>::Type;
public:
	using Base::begin;
	using Base::end;

	void appendRange(const Span<const T>& r) { Base::insert(end(), r.begin(), r.end()); }

	Span<      T> span()			{ return Span<      T>(begin(), end()); }
	Span<const T> span() const		{ return Span<const T>(begin(), end()); }

	operator Span<      T>()		{ return span(); }
	operator Span<const T>() const	{ return span(); }

	Span<      T> subspan(size_t offset, size_t count)			{ return Span<      T>(begin() + offset, count); }
	Span<const T> subspan(size_t offset, size_t count) const	{ return Span<const T>(begin() + offset, count); }

	Span<      T> subspan(size_t offset)		{ return subspan(offset, size() - offset); }
	Span<const T> subspan(size_t offset) const	{ return subspan(offset, size() - offset); }

	void remove(const T& value) { eastl::remove(begin(), end(), value); }
};

template<class KEY, class VALUE> using Map = eastl::map<KEY, VALUE>;
template<class KEY, class VALUE> using VectorMap = eastl::vector_map<KEY, VALUE>;
template<class VALUE> using StringMap = eastl::string_map<VALUE>;

template<class KEY> using Set = eastl::set<KEY>;

template<class T> using Opt = eastl::optional<T>;

template<class T> using StrViewT = eastl::basic_string_view<T>;
using StrViewA = StrViewT<char>;
using StrViewW = StrViewT<wchar_t>;

template<class T, size_t N, bool bEnableOverflow = true>
struct StringT_Base {
	using Type = typename eastl::fixed_string<T, N, bEnableOverflow>;
};

template<class T>
struct StringT_Base<T, 0, true> {
	using Type = typename eastl::basic_string<T>;
};

template<class T, size_t N, bool bEnableOverflow = true> // using FixedStringT = eastl::fixed_string<T, N, bEnableOverflow>;
class StringT : public StringT_Base<T, N, bEnableOverflow>::Type {
	using Base = typename StringT_Base<T, N, bEnableOverflow>::Type;
public:
	StringT() = default;
	StringT(const T* begin, const T* end) : Base(begin, end) {}
	StringT(StrViewT<T> view) : Base(view.data(), view.size()) {}
	StringT(StringT&& str) : Base(std::move(str)) {}
	StringT(const T* sz) : Base(sz) {}

	template<class R> void operator=(R&& r) { Base::operator=(SGE_FORWARD(r)); }

	void operator+=(StrViewT<T> v) { Base::append(v.begin(), v.end()); }

	template<size_t N>
	void operator+=(const StringT<T, N>& v) { Base::append(v.begin(), v.end()); }

	template<class R> void operator+=(const R& r) { Base::operator+=(r); }

	StrViewT<T>	view() const { return StrViewT<T>(data(), size()); }
};

template<size_t N, bool bEnableOverflow = true> using StringA_ = StringT<char,    N, bEnableOverflow>;
template<size_t N, bool bEnableOverflow = true> using StringW_ = StringT<wchar_t, N, bEnableOverflow>;

using TempStringA = StringA_<220>;
using TempStringW = StringW_<220>;

using StringA = StringA_<0>;
using StringW = StringW_<0>;

using StrView		= StrViewA;
using String		= StringA;

inline StrView StrView_c_str(const char* s) {
	return s ? StrView(s, strlen(s)) : StrView();
}

inline StrView StrView_make(ByteSpan s) {
	return StrView(reinterpret_cast<const char*>(s.data()), s.size());
}

inline ByteSpan ByteSpan_make(StrView v) {
	return ByteSpan(reinterpret_cast<const u8*>(v.data()), v.size());
}

template<size_t N> using String_ = StringA_<N>;
using TempString	= TempStringA;

template<size_t N> struct CharBySize;
template<> struct CharBySize<1> { using Type = char; };
template<> struct CharBySize<2> { using Type = char16_t; };
template<> struct CharBySize<4> { using Type = char32_t; };

struct WCharUtil {
	using Char = typename CharBySize<sizeof(wchar_t)>::Type;
	Char    toChar (wchar_t c) { return static_cast<Char>(c); }
	wchar_t toWChar(Char    c) { return static_cast<wchar_t>(c); }
};


//! Source Location
class SrcLoc {
public:
	SrcLoc() = default;
	SrcLoc(const char* file_, int line_, const char* func_)
		: file(file_)
		, func(func_)
		, line(line_)
	{}

	const char* file = "";
	const char* func = "";
	int line = 0;
};

class NonCopyable {
public:
	NonCopyable() = default;
private:
	NonCopyable(const NonCopyable&) = delete;
	void operator=(const NonCopyable&) = delete;
};

class RefCountBase : public NonCopyable {
public:
	std::atomic_int	_refCount = 0;
};

template<class T> inline void sge_delete(T* p) noexcept { delete p; }

template<class T>
class ScopedValue : public NonCopyable {
public:
	ScopedValue() = default;
	ScopedValue(T* p) : _p(p) { _oldValue = *p; }
	ScopedValue(T* p, const T& newValue) : ScopedValue(p) { *p = newValue; }

	ScopedValue(ScopedValue && r) {
		_p = r._p;
		_oldValue = r._oldValue;
		r._p = nullptr;
	}

	~ScopedValue() { discard(); }

	void discard() {
		if (_p) {
			*_p = _oldValue;
			_p = nullptr;
		}
	}

private:
	T* _p = nullptr;
	T _oldValue;
};

template<class T> ScopedValue<T> makeScopedValue(T* p) { return ScopedValue<T>(p); }
template<class T> ScopedValue<T> makeScopedValue(T* p, const T& newValue) { return ScopedValue<T>(p, newValue); }

} // namespace
