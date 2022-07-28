#pragma once

#include "JsonSerializer.h"
#include <sge_core/base/Error.h>

namespace sge {

struct JsonDeserializer : public NonCopyable {
	using This = JsonDeserializer;
public:

	JsonDeserializer(Json& json) : _json(json) {
		_stack.emplace_back(&_json);
	}

	void io(bool& v) { toValue(v); }

	void io(u8&  v) { toValue(v); }
	void io(u16& v) { toValue(v); }
	void io(u32& v) { toValue(v); }
	void io(u64& v) { toValue(v); }

	void io(i8&  v) { toValue(v); }
	void io(i16& v) { toValue(v); }
	void io(i32& v) { toValue(v); }
	void io(i64& v) { toValue(v); }

	void io(f32& v) { toValue(v); }
	void io(f64& v) { toValue(v); }

	template<class V> void io(V& v) { JsonIO<This, V>::io(*this, v); }

	template<class V> void named_io(const char* name, V& v) {
		toObjectMember(name, v);
	}

template<class SE, class T, class ENABLE> friend struct JsonIO;
protected:
	template<class V>
	void toValue(V& v) {
		auto& cur = _stack.back();
		if (cur->is_null())
			throw SGE_ERROR("value is null");
		v = *cur;
	}

	StrView toStrView() {
		auto& cur = _stack.back();
		if (!cur->is_string())
			throw SGE_ERROR("string expected");
		auto* str = cur->get_ptr<Json::string_t*>();
		return StrView(str->data(), str->size());
	}

	template<class V>
	void toString(V& v) {
		v = toStrView();
	}

	template<class V>
	void toEnum(V& v) {
		auto s = toStrView();
		if (!enumTryParse(v, s)) {
			throw SGE_ERROR("error parse enum {}", s);
		}
	}

	void beginObject() {
		auto& cur = _stack.back();
		if (!cur->is_object()) {
			throw SGE_ERROR("is not object");
		}
	}

	void endObject() {
		auto& cur = _stack.back();
		if (!cur->is_object()) {
			throw SGE_ERROR("endObject");
		}
	}

	template<class V>
	void toObjectMember(const char* name, V& v) {
		auto& cur = _stack.back();
		if(!cur->is_object())
			throw SGE_ERROR("not object member");

		auto& memberValue = cur->operator[](name);
		_stack.emplace_back(&memberValue);
		io(v);
		_stack.pop_back();
	}
	
	size_t beginArray() {
		auto& cur = _stack.back();
		if (!cur->is_array())
			throw SGE_ERROR("is not array");

		auto* arr = cur->get_ptr<Json::array_t*>();
		return arr->size();
	}

	void endArray() {
		auto& cur = _stack.back();
		if (!cur->is_array())
			throw SGE_ERROR("endArray");
	}

	template<class V>
	void toArrayElement(size_t index, V& v) {
		auto& cur = _stack.back();
		if (!cur->is_array())
			throw SGE_ERROR("not array elemnt");

		auto* arr = cur->get_ptr<Json::array_t*>();
		if (index >= arr->size())
			throw SGE_ERROR("array out of range");

		auto& elementValue = arr->at(index);
		_stack.emplace_back(&elementValue);
		io(v);
		_stack.pop_back();
	}

private:
	Json& _json;
	Vector<Json*, 64>	_stack;
};


template<class T, size_t N>
struct JsonIO <JsonDeserializer, Vector<T, N>> {
	static void io(JsonDeserializer& se, Vector<T, N>& data) {
		size_t n = se.beginArray();
		data.clear(); // ensure elements dtor get called
		data.resize(n);
		for (size_t i = 0; i < n; i++) {
			se.toArrayElement(i, data[i]);
		}
		se.endArray();
	}
};

template<size_t N>
struct JsonIO <JsonDeserializer, String_<N>> {
	static void io(JsonDeserializer& se, String_<N>& data) {
		data = se.toStrView();
	}
};


}
