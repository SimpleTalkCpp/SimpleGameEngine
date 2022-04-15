#include "FilePath.h"

namespace sge {

String FilePath::getDir(StrView path) {
	auto* end = path.end();
	auto* begin = path.begin();

	if (end == nullptr) return String();
	auto* p = end - 1;
	for (; p >= begin; p--) {
		if (*p == '/' || *p == '\\') {
			return String(begin, p);
		}
	}

	return String();
}

}