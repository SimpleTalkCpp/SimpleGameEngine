#include "Component.h"

namespace sge {

const TypeInfo* Component::s_getType() {
	class TI : public TI_Base {
	public:
		TI() {
			//static FieldInfo fi[] = {
			//};
			//setFields(fi);
		}
	};
	static TI ti;
	return &ti;
}

}
