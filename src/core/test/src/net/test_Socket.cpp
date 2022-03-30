#include <sge_core/base/UnitTest.h>
#include <sge_core/net/Socket.h>

namespace sge {

class Test_Socket : public UnitTestBase {
public:
	
	void test_resolveIPv4() {
		Vector<int> a;


		IPv4 ip;
		ip.resolve("localhost");
		SGE_DUMP_VAR(ip);
		SGE_TEST_CHECK(ip == IPv4(127,0,0,1));
		SGE_TEST_CHECK_SLIENT(ip == IPv4(127,0,0,1));
	}
};

} // namespace 

void test_Socket() {
	using namespace sge;
	SGE_TEST_CASE(Test_Socket, test_resolveIPv4());
}
