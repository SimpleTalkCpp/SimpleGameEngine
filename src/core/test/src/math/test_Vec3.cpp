#include <sge_core/base/UnitTest.h>
#include <sge_core/math/Vec3.h>

namespace sge {

template<class Vec>
class Test_Vec3 : public UnitTestBase {
public:	
	using T = typename Vec::ElementType;

	void operator_test1() {
		Vec t = Tuple3<T>(4,5,6);
		SGE_TEST_CHECK(t == Vec(4,5,6));

		Vec a(1,2,3);
		Vec b(2,4,6);
//		Vec c = a + b;

		SGE_TEST_CHECK(a + b == Vec( 3, 6, 9));
		SGE_TEST_CHECK(a - b == Vec(-1, -2, -3));
		SGE_TEST_CHECK(a * b == Vec( 2, 8, 18));
		SGE_TEST_CHECK(a / b == Vec( 0.5, 0.5, 0.5));
	}
};

} // ----- end of namespace -----

#define SGE_TEST_VEC3_CASE(...) \
	SGE_TEST_CASE(Test_Vec3< Vec3_Basic<f32> >, __VA_ARGS__); \
	SGE_TEST_CASE(Test_Vec3< Vec3_Basic<f64> >, __VA_ARGS__); \
	SGE_TEST_CASE(Test_Vec3< Vec3_SSE<  f32> >, __VA_ARGS__); \
	SGE_TEST_CASE(Test_Vec3< Vec3_SSE<  f64> >, __VA_ARGS__); \
//---

void test_Vec3() {
	using namespace sge;
// 	SGE_TEST_CASE(Test_Vec3< Vec3_Basic<f32> >, operator_test1());
 	SGE_TEST_CASE(Test_Vec3< Vec3_SSE<  f32> >, operator_test1());
// 	SGE_TEST_VEC3_CASE(operator_test1());

	return;
}
