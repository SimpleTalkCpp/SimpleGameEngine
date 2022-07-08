#pragma once

#include "Vec2_Basic.h"
#include "Vec2_SSE.h"

namespace sge {

#ifndef SGE_MATH_USE_SSE
	#error
#elif SGE_MATH_USE_SSE
	template<class T> using Vec2 = Vec2_SSE<T>;
#else
	template<class T> using Vec2 = Vec2_Basic<T>;
#endif

using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;
using Vec2i = Vec2<i32>;

}
