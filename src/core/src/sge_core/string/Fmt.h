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

#define SGE_FORMATTER_T(TEMPLATE_ARGS, CLASS) \
	} /* namespace sge */ \
	template<TEMPLATE_ARGS> \
	struct fmt::formatter<sge::CLASS> { \
		auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); } \
		static auto format(const sge::CLASS& v, fmt::format_context& ctx) { \
			v.onFormat(ctx); \
			return ctx.out(); \
		} \
	}; \
	namespace sge { \
//------

#define SGE_FORMATTER(CLASS) SGE_FORMATTER_T(SGE_EMPTY, CLASS)

#define SGE_FORMATTER_ENUM_AS_INT(T) \
	} /* namespace sge */ \
	template<> \
	struct fmt::formatter<sge::T> { \
		static auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); } \
		static auto format(const sge::T& v, fmt::format_context& ctx) { \
			return fmt::format_to(ctx.out(), "{}", sge::enumInt(v)); \
		} \
	}; \
	namespace sge { \
//-----

template<class T>
struct fmt::formatter<T, typename std::enable_if_t<std::is_enum_v<T>, char> > {
	static auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }
	static auto format(const T& v, fmt::format_context& ctx) {
		using namespace sge;
		return formatter<StrView>::format(StrView_c_str(enumStr(v)), ctx);
	}
};

namespace sge {

template<class STR, class... ARGS> inline
void FmtTo(STR& outStr, ARGS&&... args) {
	fmt::format_to(std::back_inserter(outStr), SGE_FORWARD(args)...);
}

template<class... ARGS> inline
TempString Fmt(ARGS&&... args) {
	TempString o;
	FmtTo(o, SGE_FORWARD(args)...);
	return o;
}

}


