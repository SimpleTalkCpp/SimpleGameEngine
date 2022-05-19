#pragma once

#include "AppBase.h"

namespace sge {

class ConsoleApp : public AppBase {
public:
	void run() { onRun(); }

protected:
	virtual void onRun() = 0;
};

}