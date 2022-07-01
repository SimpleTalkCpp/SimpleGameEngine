#include "Mat4.h"

namespace sge {

// explicit specialization to force VisualC check syntax in function body

#ifndef SGE_MATH_USE_SSE
	#error
#elif SGE_MATH_USE_SSE
	template Mat4_SSE<float>;
	template Mat4_SSE<double>;

#else
	template Mat4_Basic<float>;
	template Mat4_Basic<double>;
#endif



}