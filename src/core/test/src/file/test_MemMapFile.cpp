#include <sge_core/base/UnitTest.h>
#include <sge_core/file/MemMapFile.h>

namespace sge {

class Test_MemMapFile : public UnitTestBase {
public:	
	void test_open() {
		String filename = __FILE__;
		filename.append("_sample.txt");

		static const char* sample_sz = "1234ABCD";
		ByteSpan sample(reinterpret_cast<const u8*>(sample_sz), sizeof(sample_sz));

		MemMapFile mm;
		mm.open(filename);

		SGE_TEST_CHECK(sample == mm.span());
	}
};

} // namespace 

void test_MemMapFile() {
	using namespace sge;
	SGE_TEST_CASE(Test_MemMapFile, test_open());
}
