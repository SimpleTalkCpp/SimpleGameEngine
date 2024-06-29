#include <sge_core/base/UnitTest.h>
#include <sge_core/math/Quat4.h>

namespace sge {

template<class Quat4>
class Test_Quat4 : public UnitTestBase {
public:
	using T = typename Quat4::ElementType;

	void test_lerp() {

		Quat4 q1(T(1), T(2.5), T(3), T(4.5));
		Quat4 q2(T(5), T(5),   T(5), T(5)  );

		{
			auto q = Math::lerp(q1, q2, T(0.5));
			SGE_DUMP_VAR(q.x, q.y, q.z, q.w);
		}
		{
			auto q = Math::lerp(q1, q2, 0.5); // double weight
			SGE_DUMP_VAR(q.x, q.y, q.z, q.w);
		}
	}
};

} // ----- end of namespace -----

#define SGE_TEST_QUAT_CASE(...) \
	SGE_TEST_CASE(Test_Quat4< Quat4f >, __VA_ARGS__); \
	SGE_TEST_CASE(Test_Quat4< Quat4d >, __VA_ARGS__); \
//---

void test_Quat4() {
	using namespace sge;
	SGE_TEST_QUAT_CASE(test_lerp());
	return;
}
