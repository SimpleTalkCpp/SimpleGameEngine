#pragma once

#include "sge_base.h"

#define SGE_STRUCT_TYPE(T, BASE) \
private: \
	using This = T; \
	using Base = BASE; \
	class TI_Base : public TypeInfoInit<T, BASE> { \
	public: \
		TI_Base() : TypeInfoInit<T, BASE>(#T) {} \
	}; \
public: \
	static const TypeInfo* s_getType(); \
//----

#define SGE_ABSTRACT_OBJECT_TYPE_BASE(T, BASE) \
private: \
	using This = T; \
	using Base = BASE; \
public: \
	static const TypeInfo* s_getType(); \
	virtual const TypeInfo* getType() const override { return s_getType(); } \
//----

#define SGE_ABSTRACT_OBJECT_TYPE(T, BASE) \
	SGE_ABSTRACT_OBJECT_TYPE_BASE(T, BASE) \
	class TI_Base : public TypeInfoInit<T, BASE> { \
	public: \
		TI_Base() : TypeInfoInit<T, BASE>(#T, nullptr) {} \
	}; \
private: \
//-----

#define SGE_OBJECT_TYPE(T, BASE) \
	SGE_ABSTRACT_OBJECT_TYPE_BASE(T, BASE) \
	class TI_Base : public TypeInfoInit<T, BASE> { \
	public: \
		TI_Base() : TypeInfoInit<T, BASE>(#T, &TypeCreator<T>) {} \
	}; \
private: \
//-----

namespace sge {

class Object;
class TypeInfo;

template<class T> inline const TypeInfo* TypeOf() { return T::s_getType(); }
template<class T> inline const TypeInfo* TypeOf(const T& v) { return TypeOf<T>(); }



class FieldInfo {
public:
	using Getter = const void* (*)(const void* obj);
	using Setter = void (*)(void* obj, const void* value);

	template<class OBJ, class FIELD>
	FieldInfo(	const char* name_,
				FIELD OBJ::*ptr_,
				const FIELD& (*getter_)(const OBJ& obj) = nullptr,
				void (*setter_)(OBJ& obj, const FIELD& field) = nullptr)
		: name(name_)
		, fieldType(TypeOf<FIELD>())
		, offset(memberOffset(ptr_))
		, getter(reinterpret_cast<Getter>(getter_))
		, setter(reinterpret_cast<Setter>(setter_))
	{
	}

	      void* getValuePtr(      void* obj) const { return reinterpret_cast<      u8*>(obj) + offset; }
	const void* getValuePtr(const void* obj) const { return reinterpret_cast<const u8*>(obj) + offset; }

	template<class T>
	const T& getValue(const void* obj) const {
		SGE_ASSERT(TypeOf<T>() == fieldType);
		if (getter) {
			return *reinterpret_cast<const T*>(getter(obj));
		} else {
			return *reinterpret_cast<const T*>(getValuePtr(obj));
		}
	}

	template<class T>
	void setValue(void* obj, const T& value) const {
		SGE_ASSERT(TypeOf<T>() == fieldType);
		if (setter) {
			setter(obj, &value);
		} else {
			*reinterpret_cast<T*>(getValuePtr(obj)) = value;
		}
	}

	const char* name = "";
	const TypeInfo*		fieldType = nullptr;
	intptr_t			offset = 0;
	Getter				getter;
	Setter				setter;
};

class TypeInfo {
public:
	const char* name = "";
	const TypeInfo* base = nullptr;

	size_t dataSize = 0;

	Span<const FieldInfo> fields() const { return _fields; }

	using Creator = Object* (*)();

	Creator creator;

	bool isContainer = false;
	const TypeInfo* elementType = nullptr;

	Object* createObject() const {
		if (!creator) return nullptr;
		return creator();
	}

	bool isKindOf(const TypeInfo* t) const {
		auto* p = this;
		while (p) {
			if (p == t) return true;
			p = p->base;
		}
		return false;
	}

	template<class R>
	bool isKindOf() const {
		return isKindOf(TypeOf<R>());
	}

protected:
	Span<const FieldInfo>	_fields;
};

template<class T>
class TypeInfoInitNoBase : public TypeInfo {
public:
	TypeInfoInitNoBase(const char* name_) {
		name = name_;
		dataSize = sizeof(T);
	};

	template<size_t N>
	void setFields(const FieldInfo (&fi)[N]) {
		_fields = fi;
	}
};

template<class T> inline
static Object* TypeCreator() {
	return new T();
}

template<class T, class Base>
class TypeInfoInit : public TypeInfoInitNoBase<T> {
public:
	TypeInfoInit(const char* name_, Creator creator_)
		: TypeInfoInitNoBase<T>(name_)
	{
		static_assert(std::is_base_of<Base, T>::value, "invalid base class");
		this->base = TypeOf<Base>();
		this->creator = creator_;
	};
};

#define SGE_TYPEOF_SIMPLE(T) \
	template<> const TypeInfo* TypeOf<T>();
//----

#define SGE_TYPEOF_SIMPLE_IMP(T, NAME) \
	template<> const TypeInfo* TypeOf<T>() { \
		static TypeInfoInitNoBase<T> ti(NAME); \
		return &ti; \
	} \
//----

SGE_TYPEOF_SIMPLE(float)
SGE_TYPEOF_SIMPLE(double)

SGE_TYPEOF_SIMPLE(int8_t)
SGE_TYPEOF_SIMPLE(int16_t)
SGE_TYPEOF_SIMPLE(int32_t)
SGE_TYPEOF_SIMPLE(int64_t)

SGE_TYPEOF_SIMPLE(uint8_t)
SGE_TYPEOF_SIMPLE(uint16_t)
SGE_TYPEOF_SIMPLE(uint32_t)
SGE_TYPEOF_SIMPLE(uint64_t)

SGE_TYPEOF_SIMPLE(char8_t) // c++20
SGE_TYPEOF_SIMPLE(char16_t)
SGE_TYPEOF_SIMPLE(char32_t)
SGE_TYPEOF_SIMPLE(wchar_t)

}