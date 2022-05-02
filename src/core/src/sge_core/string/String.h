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

	static void appendBinToHex(String& result, Span<const u8> data);
	
	static bool hasChar(StrView view, char ch) { return StrView::npos != view.find(ch); }

	static std::pair<StrView, StrView> splitByChar	(StrView view, StrView seperators);
	static std::pair<StrView, StrView> splitByChar	(StrView view, char seperator);

	static StrView	trimChar(StrView view, StrView seperators);

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
struct fmt::formatter<sge::StrView> {
	auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }
	auto format(const sge::StrView& v, fmt::format_context& ctx) {
		auto it = *ctx.out();
		for (const auto& c : v) {
			it = c;
			it++;
		}
		return ctx.out();
	}
};

template<>
struct fmt::formatter<sge::String> {
	auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }
	auto format(const sge::String& v, fmt::format_context& ctx) {
		sge::StrView view(v.data(), v.size());
		return fmt::format_to(ctx.out(), "{}", view);
	}
};

template<size_t N>
struct fmt::formatter<sge::String_<N>> {
	auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }
	auto format(const sge::String_<N>& v, fmt::format_context& ctx) {
		sge::StrView view(v.data(), v.size());
		return fmt::format_to(ctx.out(), "{}", view);
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

template<>
struct fmt::formatter<sge::StringW> {
	auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }
	auto format(const sge::StringW& v, fmt::format_context& ctx) {
		sge::StrViewW view(v.data(), v.size());
		return fmt::format_to(ctx.out(), "{}", view);
	}
};

template<size_t N>
struct fmt::formatter<sge::StringW_<N>> {
	auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }
	auto format(const sge::StringW_<N>& v, fmt::format_context& ctx) {
		sge::StrViewW view(v.data(), v.size());
		return fmt::format_to(ctx.out(), "{}", view);
	}
};
