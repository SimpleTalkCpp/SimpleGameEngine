#pragma once

#include "sge_base.h"

namespace sge {

class Object;
class TypeInfo;

template<class T> const TypeInfo* TypeInfo_get();

class FieldInfo {
public:

	template<class OBJ, class FIELD>
	FieldInfo(const char* name_, FIELD OBJ::*ptr)
		: name(name_)
		, fieldType(TypeInfo_get<FIELD>())
		, offset(TypeInfo_get(ptr))
	{
	}

	const char* name = "";
	const TypeInfo* fieldType = nullptr;
	intptr_t offset = 0;
};

class FieldsEnumerator {
public:
	FieldsEnumerator(const TypeInfo* typeInfo_) 
		: typeInfo(typeInfo_) 
	{}

	class Iterator {
	public:
		Iterator(const TypeInfo* typeInfo_, size_t fieldIndex_)
			: typeInfo(typeInfo_)
			, fieldIndex(fieldIndex_)
		{
		}

		bool operator!=(const Iterator& r) const {
			return typeInfo != r.typeInfo || fieldIndex != r.fieldIndex;
		}

		void operator++();

		const FieldInfo& operator*();

	private:
		const TypeInfo* typeInfo = nullptr;
		size_t fieldIndex = 0;
	};

	Iterator begin()	{ return Iterator(typeInfo, 0); }
	Iterator end()		{ return Iterator(nullptr, 0); }

	const TypeInfo* typeInfo = nullptr;
};

class TypeInfo {
public:
	const char* name = "";
	const TypeInfo* base = nullptr;

	size_t dataSize = 0;

	const FieldInfo* fieldsArray = nullptr;
	size_t fieldCount = 0;

	FieldsEnumerator fields() const { return FieldsEnumerator(this); }

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
		return isKindOf(TypeInfo_get<R>());
	}
};

}