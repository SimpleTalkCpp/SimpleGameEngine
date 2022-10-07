#include "Object.h"

namespace sge {

template<> const TypeInfo* TypeInfo_get<Object>() {
	class TI : public TypeInfo {
	public:
		TI() {
			name = "Object";
		}
	};
	static TI ti;
	return &ti;
}

}
