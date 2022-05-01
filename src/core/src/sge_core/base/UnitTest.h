#pragma once

#include <sge_core/base/sge_base.h>
#include <sge_core/log/Log.h>

#define	SGE_TEST_CASE(TestClass, TestFunc) \
	do{ \
		SGE_LOG("\n[--TEST_CASE--] " #TestClass "." #TestFunc "\n"); \
		TestClass testObj; \
		testObj.TestFunc; \
	}while(false)
//----

#define SGE_TEST_CHECK(a)			SGE_TEST_CHECK_IMPL(SGE_LOC, true,  bool(a), #a)
#define SGE_TEST_CHECK_SLIENT(a)	SGE_TEST_CHECK_IMPL(SGE_LOC, false, bool(a), #a)

namespace sge {

bool SGE_TEST_CHECK_IMPL(const SrcLoc& loc, bool verbose, bool success, const char* msg);

class UnitTestBase : public NonCopyable {
public:
};

}