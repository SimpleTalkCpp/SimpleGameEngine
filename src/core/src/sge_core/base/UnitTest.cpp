#include "UnitTest.h"
#include "../string/StringUtil.h"

namespace sge {

bool SGE_TEST_CHECK_IMPL(const SrcLoc& loc, bool verbose, bool success, const char* msg) {
	if (success && !verbose)
		return success;

	auto str = fmt::format("{} {}\n", (success ? "[ OK ]" : "[FAIL]"), msg);
	std::cout << str;
	if (!success) {
		assert(false);
	}
	return success;
}

} // namespace