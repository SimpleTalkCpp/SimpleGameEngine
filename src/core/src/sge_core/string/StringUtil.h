#pragma once

#include "../base/sge_base.h"
#include "UtfUtil.h"
#include "Fmt.h"

namespace sge {

struct StringUtil {
	StringUtil() = delete;

	static void binToHex(String& result, Span<u8> data) {
		result.clear();
		appendBinToHex(result, data);
	}

	static void appendBinToHex(String& result, ByteSpan data);
	
	static bool hasChar(StrView view, char ch) { return StrView::npos != view.find(ch); }

	static std::pair<StrView, StrView> splitByChar	(StrView view, StrView seperators);
	static std::pair<StrView, StrView> splitByChar	(StrView view, char seperator);

	static StrView	trimChar(StrView view, StrView seperators);

	static const char* findChar			(StrView view, StrView charList, bool ignoreCase);
	static const char* findCharFromEnd	(StrView view, StrView charList, bool ignoreCase);

	static bool ignoreCaseCompare(char a, char b) { return tolower(a) == tolower(b); }

	static bool tryParse(StrView view, i8 & outValue);
	static bool tryParse(StrView view, i16& outValue);
	static bool tryParse(StrView view, i32& outValue);
	static bool tryParse(StrView view, i64& outValue);

	static bool tryParse(StrView view, u8 & outValue);
	static bool tryParse(StrView view, u16& outValue);
	static bool tryParse(StrView view, u32& outValue);
	static bool tryParse(StrView view, u64& outValue);

	static bool tryParse(StrView view, f32& outValue);
	static bool tryParse(StrView view, f64& outValue);
};

inline
std::pair<StrView, StrView> StringUtil::splitByChar(StrView view, StrView seperators) {
	auto* s = view.begin();
	auto* e = view.end();
	for (auto* p = s; p < e; p++) {
		if (hasChar(seperators, *p)) {
			auto r0 = StrView(s,   p-s);
			auto r1 = StrView(p+1, e-p-1);
			return {r0, r1};
		}
	}
	return {view, StrView()};
}

inline
std::pair<sge::StrView, sge::StrView> StringUtil::splitByChar(StrView view, char seperator) {
	return splitByChar(view, StrView(&seperator, 1));
}

inline
StrView StringUtil::trimChar(StrView view, StrView charList) {
	auto* p = view.begin();
	auto* e = view.end();
	for (; p < e; p++) {
		if (!hasChar(charList, *p)) break;
	}
	return StrView(p, e-p);
}

} // namespace

inline
std::ostream& operator<<(std::ostream& s, const sge::StrView& v) {
	s.write(v.data(), v.size());
	return s;
}

template<>
struct fmt::formatter<sge::StrViewA> {
	auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }
	auto format(const sge::StrViewA& v, fmt::format_context& ctx) {
		auto it = *ctx.out();
		for (const auto& c : v) {
			it = c;
			it++;
		}
		return ctx.out();
	}
};

template<>
struct fmt::formatter<sge::StrViewW> {
	auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }
	auto format(const sge::StrViewW& v, fmt::format_context& ctx) {
		sge::TempStringA tmp = sge::UtfUtil::toString(v);
		return fmt::format_to(ctx.out(), "{}", tmp);
	}
};

template<class T, size_t N, bool bEnableOverflow>
struct fmt::formatter<sge::StringT<T, N, bEnableOverflow> > {
	auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }
	auto format(const sge::StringT<T, N, bEnableOverflow>& v, fmt::format_context& ctx) {
		return fmt::format_to(ctx.out(), "{}", v.view());
	}
};