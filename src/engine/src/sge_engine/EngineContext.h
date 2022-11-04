#pragma once

#include <sge_engine/components/CSystemBase.h>

namespace sge {

class EngineContext : public NonCopyable {
public:
	static EngineContext*	create();
	static void				destroy();

	static EngineContext*	instance() { return s_instance; }

	template<class C>
	void registerComponentType() { _registerComponentType(TypeOf<C>()); }
	void _registerComponentType(const TypeInfo* type);


	template<class C>
	void registerSystem() { _registerSystem(new C); }
	void _registerSystem(CSystemBase* sys);

private:
	static EngineContext*	s_instance;

	StringMap<const TypeInfo*>	_componentTypeMap;
	Vector< UPtr<CSystemBase> >	_systems;

	EngineContext();
};

}
