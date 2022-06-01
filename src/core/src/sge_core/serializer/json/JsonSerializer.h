#pragma once

#include "Json.h"

namespace sge {

struct JsonSerializer : public NonCopyable {
	JsonSerializer(String& outStr) : _out(outStr) {}

	template<class D>
	void io(D& data) {
	}

private:
	String&	_out;
};

}
