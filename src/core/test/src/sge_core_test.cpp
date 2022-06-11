#include <sge_core/base/UnitTest.h>
#include <sge_core/log/Log.h>

// SGE_OPTIMIZE_OFF

#define	RUN_TEST( fn )	\
	SGE_LOG("\n===== " #fn " ================================\n"); \
	void fn(); \
	fn(); \
//----

namespace sge {

void run_temp_test() {
	//RUN_TEST(test_Vec2);
	RUN_TEST(test_Vec3);
	//RUN_TEST(test_Vec4);
	//RUN_TEST(test_Mat4);
}

void run_all_test() {
// base
	RUN_TEST(test_BinSerializer);

// file
	RUN_TEST(test_MemMapFile);

// math
	RUN_TEST(test_Vec2);
	RUN_TEST(test_Vec3);
	RUN_TEST(test_Vec4);
	RUN_TEST(test_Mat4);

// string
	RUN_TEST(test_Fmt);
	RUN_TEST(test_UtfUtil);

// net
	RUN_TEST(test_Socket);
}

int test_main() {
#if 1
	run_temp_test();
#else
	run_all_test();
#endif
	// TODO: check memory leak

	SGE_LOG("\n\n==== Program Ended ==== \n");
	return 0;
}

} // namespace

int main() {
	return sge::test_main();
}
