#pragma once

#include "Vec2.h"

namespace sge {

// explicit specialization to force VisualC check syntax in function body
template Vec2<float>;
template Vec2<double>;

}