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

		SGE_TEST_CHECK(a + b == Vec( 3, 6, 9));
		SGE_TEST_CHECK(a - b == Vec(-1, -2, -3));
		SGE_TEST_CHECK(a * b == Vec( 2, 8, 18));
		SGE_TEST_CHECK(a / b == Vec( 0.5, 0.5, 0.5));

		{ Vec c = a; c += b; SGE_TEST_CHECK(c == a + b); }
		{ Vec c = a; c -= b; SGE_TEST_CHECK(c == a - b); }
		{ Vec c = a; c *= b; SGE_TEST_CHECK(c == a * b); }
		{ Vec c = a; c /= b; SGE_TEST_CHECK(c == a / b); }

		T s = 2;
		SGE_TEST_CHECK(a + s == Vec(3,4,5));
		SGE_TEST_CHECK(a - s == Vec(-1,0,1));
		SGE_TEST_CHECK(a * s == Vec(2,4,6));
		SGE_TEST_CHECK(a / s == Vec(0.5, 1, 1.5));

		{ Vec c = a; c += s; SGE_TEST_CHECK(c == a + s); }
		{ Vec c = a; c -= s; SGE_TEST_CHECK(c == a - s); }
		{ Vec c = a; c *= s; SGE_TEST_CHECK(c == a * s); }
		{ Vec c = a; c /= s; SGE_TEST_CHECK(c == a / s); }

	}
};

} // ----- end of namespace -----

#define SGE_TEST_VEC3_CASE(...) \
	SGE_TEST_CASE(Test_Vec3< Vec3f_Basic>, __VA_ARGS__); \
	SGE_TEST_CASE(Test_Vec3< Vec3f_SSE  >, __VA_ARGS__); \
	\
	SGE_TEST_CASE(Test_Vec3< Vec3d_Basic>, __VA_ARGS__); \
	SGE_TEST_CASE(Test_Vec3< Vec3d_SSE  >, __VA_ARGS__); \
//---

void test_Vec3() {
	using namespace sge;
 	SGE_TEST_VEC3_CASE(operator_test1());
	return;
}
