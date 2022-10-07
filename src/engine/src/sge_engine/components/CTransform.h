#pragma once

#include "Component.h"

namespace sge {

class CTransform : public Component {
public:

	Vec3f	position{0,0,0};
	Quat4f	rotation{0,0,0,0};
	Vec3f	scale{1,1,1};
};

}