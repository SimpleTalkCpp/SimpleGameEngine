#include <sge_core/base/UnitTest.h>
#include <sge_core/math/Vec4.h>

namespace sge {

template<class Vec>
class Test_Vec4 : public UnitTestBase {
public:	
	using T = typename Vec::ElementType;

	void operator_test1() {
		Vec t = Tuple4<T>(4,5,6,7);
		SGE_TEST_CHECK(t == Vec(4,5,6,7));

		Vec a(1,2,3,4);
		Vec b(2,4,6,8);
//		Vec c = a + b;

		SGE_TEST_CHECK(a + b == Vec( 3, 6, 9, 12));
		SGE_TEST_CHECK(a - b == Vec(-1, -2, -3, -4));
		SGE_TEST_CHECK(a * b == Vec( 2, 8, 18, 32));
		SGE_TEST_CHECK(a / b == Vec( 0.5, 0.5, 0.5, 0.5));
	}
};

} // ----- end of namespace -----

#define SGE_TEST_Vec4_CASE(...) \
	SGE_TEST_CASE(Test_Vec4< Vec4_Basic<f32> >, __VA_ARGS__); \
	SGE_TEST_CASE(Test_Vec4< Vec4_Basic<f64> >, __VA_ARGS__); \
	SGE_TEST_CASE(Test_Vec4< Vec4_SSE<  f32> >, __VA_ARGS__); \
	SGE_TEST_CASE(Test_Vec4< Vec4_SSE<  f64> >, __VA_ARGS__); \
//---

void test_Vec4() {
	using namespace sge;
// 	SGE_TEST_CASE(Test_Vec4< Vec4_Basic<f32> >, operator_test1());
 	SGE_TEST_CASE(Test_Vec4< Vec4_SSE<  f32> >, operator_test1());
// 	SGE_TEST_Vec4_CASE(operator_test1());

	return;
}
