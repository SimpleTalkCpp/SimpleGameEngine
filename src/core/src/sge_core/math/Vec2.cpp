#include "Vec2.h"

namespace sge {

// explicit specialization to force VisualC check syntax in function body

#ifndef SGE_MATH_USE_SSE
	#error
#elif SGE_MATH_USE_SSE
	template Vec2_SSE<float>;
	template Vec2_SSE<double>;

#else
	template Vec2_Basic<float>;
	template Vec2_Basic<double>;
#endif

}