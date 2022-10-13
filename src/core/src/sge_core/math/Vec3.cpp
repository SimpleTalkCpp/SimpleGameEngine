#include "Vec3.h"

namespace sge {

// explicit specialization to force VisualC check syntax in function body

#ifndef SGE_MATH_USE_SSE
	#error
#elif SGE_MATH_USE_SSE
	template Vec3_SSE<float>;
	template Vec3_SSE<double>;

#else
	template Vec3_Basic<float>;
	template Vec3_Basic<double>;
#endif

template<> const TypeInfo* TypeOf<Vec3f>() {
	using This = Vec3f;
	using TI_Base = TypeInfoInitNoBase<This>;

	class TI : public TI_Base {
	public:
		TI() : TI_Base("Vec3f") {
			static FieldInfo fi[] = {
				FieldInfo("x", &This::x),
				FieldInfo("y", &This::y),
				FieldInfo("z", &This::z),
			};
			setFields(fi);
		}
	};
	static TI ti;
	return &ti;
}

}