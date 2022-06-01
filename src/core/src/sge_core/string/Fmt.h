#pragma once

#include "../base/sge_base.h"

#include <fmt/format.h>

#if SGE_COMPILER_VC
	#if _DEBUG
		#pragma comment(lib, "fmtd.lib")
	#else
		#pragma comment(lib, "fmt.lib")
	#endif
#endif

#define SGE_FORMATTER(T) \
	} /* namespace sge */ \
	template<> \
	struct fmt::formatter<sge::T> { \
		auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); } \
		auto format(const sge::T& v, fmt::format_context& ctx) { \
			v.onFormat(ctx); \
			return ctx.out(); \
		} \
	}; \
	namespace sge { \
//------

#define SGE_FORMATTER_ENUM(T) \
	} /* namespace sge */ \
	template<> \
	struct fmt::formatter<sge::T> { \
		auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); } \
		auto format(const sge::T& v, fmt::format_context& ctx) { \
			return fmt::format_to(ctx.out(), "{}", sge::enumStr(v)); \
		} \
	}; \
	namespace sge { \
//-----

namespace sge {

template<class... ARGS> inline
void FmtTo(TempString& outStr, ARGS&&... args) {
	fmt::format_to(std::back_inserter(outStr), SGE_FORWARD(args)...);
}

template<class... ARGS> inline
TempString Fmt(ARGS&&... args) {
	TempString o;
	FmtTo(o, SGE_FORWARD(args)...);
	return o;
}

}


