#pragma once

#include "Component.h"

namespace sge {

class CTransform : public Component {
	SGE_OBJECT_TYPE(CTransform, Component)
public:
	Vec3f	position {0,0,0};
	Quat4f	rotate {0,0,0,0};
	Vec3f	scale {1,1,1};
};

}