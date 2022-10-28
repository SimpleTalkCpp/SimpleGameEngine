#include "Entity.h"
#include "Scene.h"

namespace sge {

const TypeInfo* Entity::s_getType() {
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

Entity::Entity() {
	_transform = addComponent<CTransform>();
}

}
