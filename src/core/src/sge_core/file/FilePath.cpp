#include "FilePath.h"
#include "Directory.h"
#include <sge_core/string/StringUtil.h>

namespace sge {

StrView FilePath::dirname(StrView path) {
	auto* end = path.end();
	auto* begin = path.begin();

	if (end == nullptr) return StrView();
	auto* p = end - 1;
	for (; p >= begin; p--) {
		if (*p == '/' || *p == '\\') {
			return StrView(begin, p - begin);
		}
	}

	return StrView();
}

StrView FilePath::basename(StrView path) {
	auto* end = path.end();
	auto* begin = path.begin();

	if (end == nullptr) return StrView();
	auto* p = end - 1;
	for (; p >= begin; p--) {
		if (*p == '/' || *p == '\\') {
			p++;
			return StrView(p, end - p);
		}
	}

	return StrView();
}

StrView FilePath::extension(StrView path) {
	auto* end = path.end();
	auto* begin = path.begin();

	if (end == nullptr) return StrView();
	auto* p = end - 1;
	for (; p >= begin; p--) {
		if (*p == '.') {
			p++;
			return StrView(p, end - p);
		}
	}

	return StrView();
}

bool FilePath::isRealpath(const StrView& path) {
	if (path.size() < 1) return false;
	if (path[0] == '/') return true;

	if (path.size() < 2) return false;
	if (std::isalpha(path[0]) && path[1] == ':') return true;

	return false;
}

String FilePath::realpath(StrView path) {
	if (!path.size()) return String();

	String outStr;

	bool needSlash = false;
	if (isRealpath(path)) {
		needSlash = (path[0] == '/'); //UNIX path need '/' at beginning

	}else{
		outStr = Directory::getCurrent();
		needSlash = true;
	}

	StrView p = path;
	while (p.size()) {
		auto s = StringUtil::splitByChar(p, "\\/");
		if (s.first == ".") {
			//skip '.'
		}else if (!s.first.size()) {
			//skip '/'
		}else if (s.first == "..") {
			auto* parent = StringUtil::findCharFromEnd(outStr, "\\/", false);
			if (parent == nullptr) {
				outStr.clear(); //no more parent folder
				return String();
			}

			outStr.resize(parent - outStr.data());
		}else{
			if (needSlash) {
				outStr += '/';
			}
			outStr += s.first;
			needSlash = true;
		}
		p = s.second;
	}

	return outStr;
}

}