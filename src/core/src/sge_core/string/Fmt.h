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
	template<> \
	struct fmt::formatter<T> { \
		auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); } \
		auto format(const T& v, fmt::format_context& ctx) { return v.onFormat(ctx); } \
	};
//------

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


