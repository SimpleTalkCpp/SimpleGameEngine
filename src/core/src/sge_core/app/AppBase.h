#pragma once

#include <sge_core/string/UtfUtil.h>

namespace sge {

class AppBase : public NonCopyable {
public:

	void	setCurrentDir(StrView dir);
	String	getCurrentDir();
	String	getExecutableFilename();

protected:

};

}