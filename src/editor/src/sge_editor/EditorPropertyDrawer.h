#pragma once

namespace sge {

struct EditorPropertyDrawRequest {
	const FieldInfo*	field = nullptr;
	const TypeInfo*		objectType = nullptr;
	Vector<void*, 16>	objects;

	template<class T>
	const T& getFirstObjectValue() { return field->getValue<T>(objects[0]); }

	template<class T>
	bool isMixedValue() {
		if (objects.size() <= 1)
			return false;

		auto& v = field->getValue<T>(objects[0]);
		for (size_t i = 1; i < objects.size(); i++) {
			if (v != field->getValue<T>(objects[i]))
				return true;
		}
		return false;
	}

	template<class T>
	void setAllObjectsValue(const T& v) {
		for (auto& o : objects) {
			field->setValue<T>(o, v);
		}
	}

	void createChildRequest(EditorPropertyDrawRequest& cr) {
		cr.objectType = field->fieldType;
		cr.objects.clear();
		cr.objects.reserve(objects.size());
		for (auto& o : objects) {
			cr.objects.emplace_back(field->getValuePtr(o));
		}
	}
};

class EditorPropertyDrawer : public NonCopyable {
public:
	using DrawRequest = EditorPropertyDrawRequest;
	virtual void draw(DrawRequest& req) {}
};

class EditorPropertyDrawer_struct : public EditorPropertyDrawer {
public:
	virtual void draw(DrawRequest& req) override;
};

class EditorPropertyDrawer_float : public EditorPropertyDrawer {
public:
	virtual void draw(DrawRequest& req) override;
};

class EditorPropertyDrawer_Vec3f : public EditorPropertyDrawer {
public:
	virtual void draw(DrawRequest& req) override;
};


}