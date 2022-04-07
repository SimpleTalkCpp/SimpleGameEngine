#include <sge_core/base/UnitTest.h>
#include <sge_core/string/UtfUtil.h>

#include <fstream>
#include <sstream>

namespace sge {

class Test_UtfUtil : public UnitTestBase {
public:	
	void test() {
		String filename(__FILE__);
		filename.append("_sample.txt");

		std::ifstream inFile(filename.c_str());
		std::stringstream strStream;
		strStream << inFile.rdbuf(); //read the file

		const StringA src = strStream.str().c_str();

		StringW w;
		StringA a;

		UtfUtil::convert(w, src);
		SGE_DUMP_VAR(w);

		UtfUtil::convert(a, w);
		SGE_DUMP_VAR(a);
		SGE_TEST_CHECK(a == src);
	}
};

} // namespace 

void test_UtfUtil() {
	using namespace sge;
	SGE_TEST_CASE(Test_UtfUtil, test());
}
