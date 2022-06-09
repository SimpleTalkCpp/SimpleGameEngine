#include <sge_core/base/UnitTest.h>
#include <sge_core/math/Mat4.h>

namespace sge {

template<class Mat4>
class Test_Mat4 : public UnitTestBase {
public:	
	using T = typename Mat4::ElementType;

	void operator_test1() {
		Mat4 m = Mat4::s_identity();
		m += 1;

		SGE_TEST_CHECK(m == Mat4({2,1,1,1}, 
								 {1,2,1,1},
								 {1,1,2,1},
								 {1,1,1,2}));

		SGE_DUMP_VAR(m);
	}
};

} // ----- end of namespace -----

#define SGE_TEST_MAT4_CASE(...) \
	SGE_TEST_CASE(Test_Mat4< Mat4f_Basic>, __VA_ARGS__); \
	//SGE_TEST_CASE(Test_Mat4< Mat4d_Basic>, __VA_ARGS__); \
	//\
	//SGE_TEST_CASE(Test_Mat4< Mat4f_SSE  >, __VA_ARGS__); \
	//SGE_TEST_CASE(Test_Mat4< Mat4d_SSE  >, __VA_ARGS__); \
//---

void test_Mat4() {
	using namespace sge;
 	SGE_TEST_MAT4_CASE(operator_test1());

	return;
}
