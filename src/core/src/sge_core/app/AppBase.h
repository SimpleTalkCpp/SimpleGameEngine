#pragma once

#include <sge_core/string/UtfUtil.h>

namespace sge {

class AppBase : public NonCopyable {
public:

	String	getExecutableFilename();

protected:

};

}