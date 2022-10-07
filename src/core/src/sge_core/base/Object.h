#pragma once

#include "Reflection.h"

namespace sge {

class Object;
template<> const TypeInfo* TypeInfo_get<Object>();

class Object : public RefCountBase {
public:
	virtual ~Object() = default;
	virtual const TypeInfo* getType() { return TypeInfo_get<Object>(); }
};

}
