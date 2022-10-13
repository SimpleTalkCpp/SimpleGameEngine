#include "CTransform.h"

namespace sge {

const TypeInfo* CTransform::s_getType() {
	class TI : public TI_Base {
	public:
		TI() {
			static FieldInfo fi[] = {
				FieldInfo("position",	&This::position),
				FieldInfo("rotate",		&This::rotate),
				FieldInfo("scale",		&This::scale),
			};
			setFields(fi);
		}
	};
	static TI ti;
	return &ti;

}

}