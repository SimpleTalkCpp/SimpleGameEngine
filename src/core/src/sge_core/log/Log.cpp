#include <sge_core/log/Log.h>
#include <iostream>

namespace sge {

Log g_log;

void Log::onWrite(Level lv, StrView str) {
	std::cout << str << "\n";
}

} // namespace