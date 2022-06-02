#pragma once

namespace sge {

template<class SE, class T, class ENABLE = void>
struct JsonIO {
	static void io(SE& se, T& data) {
		se.beginObject();
		data.onJson(se);
		se.endObject();
	}
};

// enum class
template<class SE, class T>
struct JsonIO <SE, T, std::enable_if_t<std::is_enum_v<T>> > {
	static void io(SE& se, T& data) {
		se.toEnum(data);
	}	
};

struct JsonSerializer : public NonCopyable {
	using This = JsonSerializer;

	JsonSerializer(Json& outJson) : _json(outJson) {
		_stack.emplace_back(&_json);
	}

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
	void toValue(const V& v) {
		auto& cur = _stack.back();
		SGE_ASSERT(cur->is_null());
		*cur = v;
	}

	void toStrView(StrView v) {
		auto& cur = _stack.back();
		SGE_ASSERT(cur->is_null());
		*cur = "";
		auto* dst = cur->get_ptr<Json::string_t*>();
		dst->assign(v.begin(), v.end());
	}

	template<class V>
	void toEnum(const V& v) {
		toValue(enumStr(v));
	}

	void beginObject() {
		auto& cur = _stack.back();
		SGE_ASSERT(cur->is_null());
		*cur = Json::object();
	}

	void endObject() {
		SGE_ASSERT(_stack.back()->is_object());
	}

	template<class V>
	void toObjectMember(const char* name, V& v) {
		auto& obj = _stack.back();
		SGE_ASSERT(obj->is_object());
		auto& memberValue = obj->operator[](name);
		_stack.emplace_back(&memberValue);

		io(v);

		_stack.pop_back();
		SGE_ASSERT(_stack.back()->is_object());
	}
	
	void beginArray(size_t size) {
		auto& arr = _stack.back();
		SGE_ASSERT(arr->is_null());
		*arr = Json::array();
	}

	void resizeArray(size_t size) {
		SGE_ASSERT(_stack.back()->is_array());
		auto* arr = _stack.back()->get_ptr<Json::array_t*>();
		arr->resize(size);
	}

	void endArray() {
		SGE_ASSERT(_stack.back()->is_array());
	}

	template<class V>
	void toArrayElement(size_t index, V& v) {
		SGE_ASSERT(_stack.back()->is_array());
		auto* arr = _stack.back()->get_ptr<Json::array_t*>();
		if (index >= arr->size())
			throw SGE_ERROR("array index out of array");
		auto& elementValue = arr->at(index);
		_stack.emplace_back(&elementValue);

		io(v);

		_stack.pop_back();
		SGE_ASSERT(_stack.back()->is_array());
	}

private:
	Json&	_json;
	Vector_<Json*, 64>	_stack;
};

template<class T, size_t N>
struct JsonIO <JsonSerializer, Vector_<T, N>> {
	static void io(JsonSerializer& se, Vector_<T, N>& data) {
		size_t n = data.size();
		se.beginArray(n);
		for (size_t i = 0; i < n; i++) {
			se.toArrayElement(i, data[i]);
		}
		se.endArray();
	}
};

template<size_t N>
struct JsonIO <JsonSerializer, String_<N>> {
	static void io(JsonSerializer& se, String_<N>& data) {
		se.toStrView(data);
	}
};

}
