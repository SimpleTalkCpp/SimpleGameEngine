#pragma once

#include "Vec3.h"
#include "Vec4_Basic.h"
#include "Vec4_SSE.h"

namespace sge {

#ifndef SGE_MATH_USE_SSE
	#error
#elif SGE_MATH_USE_SSE
	template<class T> using Vec4 = Vec4_SSE<T>;
#else
	template<class T> using Vec4 = Vec4_Basic<T>;
#endif

using Vec4f = Vec4<float>;
using Vec4d = Vec4<double>;

}
