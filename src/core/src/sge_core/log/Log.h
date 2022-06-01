#pragma once

#include <sge_core/base/sge_base.h>
#include <sge_core/string/Fmt.h>
#include <sge_core/string/StringUtil.h>

#define SGE_LOG(...)       do{ sge::g_log.write(sge::Log::Level::Info,    __VA_ARGS__); } while(false)
#define SGE_LOG_WARN(...)  do{ sge::g_log.write(sge::Log::Level::Warning, __VA_ARGS__); } while(false)
#define SGE_LOG_ERROR(...) do{ sge::g_log.write(sge::Log::Level::Error,   __VA_ARGS__); } while(false)

namespace sge {

class Log : public NonCopyable {
public:

	enum class Level {
		Unknown,
		Info,
		Warning,
		Error,
	};

	template<class... Args>
	void write(Level lv, Args&&... args) {
		TempString tmp;
		FmtTo(tmp, SGE_FORWARD(args)...);
		onWrite(lv, tmp);
	}

	void onWrite(Level lv, StrView str);
};

extern Log g_log;

} //namespace


