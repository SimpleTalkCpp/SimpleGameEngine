#include "ProjectSettings.h"
#include <sge_core/file/Directory.h>

namespace sge {

ProjectSettings* ProjectSettings::instance() {
	static ProjectSettings s;
	return &s;
}

void ProjectSettings::setProjectRoot(StrView path) {
	Directory::setCurrent(path);

	_projectRoot = path;

	//auto dir = Directory::getCurrent();
	//SGE_LOG("projectRoot = {}", dir);
}

StrView ProjectSettings::importedPath() const {
	return "LocalTemp/Imported";
}

}