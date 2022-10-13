#pragma once

namespace sge {

class EngineContext {
public:
	static EngineContext*	create();
	static void				destroy();

	static EngineContext*	instance() { return s_instance; }

	template<class C>
	void registerComponentType() { _registerComponentType(TypeOf<C>()); }
	void _registerComponentType(const TypeInfo* type);

private:
	static EngineContext*	s_instance;

	StringMap<const TypeInfo*>	_componentTypeMap;

	EngineContext();
};

}
