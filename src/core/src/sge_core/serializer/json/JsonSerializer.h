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
		se.writeValue(enumInt(data));
	}	
};

template<class SE, class T, size_t N>
struct JsonIO <SE, Vector_<T, N>> {
	static void io(SE& se, Vector_<T, N>& data) {
		se.beginArray();
		for (auto& element : data) {
			se.beginArrayElement();
			se.io(element);
			se.endArrayElement();
		}
		se.endArray();
	}
};

template<class SE, size_t N>
struct JsonIO <SE, String_<N>> {
	static void io(SE& se, String_<N>& data) {
		se.writeValue(data.c_str());
	}
};


struct JsonSerializer : public NonCopyable {
	using This = JsonSerializer;

	JsonSerializer(Json& outJson) 
		: _out(outJson) 
	{
		_stack.emplace_back(&outJson);
	}

	void io(u8&  v) { writeValue(v); }
	void io(u16& v) { writeValue(v); }
	void io(u32& v) { writeValue(v); }
	void io(u64& v) { writeValue(v); }

	void io(i8&  v) { writeValue(v); }
	void io(i16& v) { writeValue(v); }
	void io(i32& v) { writeValue(v); }
	void io(i64& v) { writeValue(v); }

	void io(f32& v) { writeValue(v); }
	void io(f64& v) { writeValue(v); }

	template<class D> void io(D& v) {
		JsonIO<This, D>::io(*this, v);
	}

	template<class D> void named_io(const char* name, D& v) {
		beginObjectMember(name);
		io(v);
		endObjectMember();
	}


template<class SE, class T, class ENABLE> friend struct JsonIO;
protected:

	template<class V>
	void writeValue(const V& v) {
		auto& cur = _stack.back();
		SGE_ASSERT(cur->is_null());
		*cur = v;
	}

	void beginObject() {
		auto& cur = _stack.back();
		SGE_ASSERT(cur->is_null());
		*cur = Json::object();
	}

	void endObject() {
		SGE_ASSERT(_stack.back()->is_object());
	}

	void beginObjectMember(const char* name) {
		auto& obj = _stack.back();
		SGE_ASSERT(obj->is_object());
		auto& memberValue = obj->operator[](name);
		_stack.emplace_back(&memberValue);
	}

	void endObjectMember() {
		_stack.pop_back();
		SGE_ASSERT(_stack.back()->is_object());
	}
	
	void beginArray() {
		auto& arr = _stack.back();
		SGE_ASSERT(arr->is_null());
		*arr = Json::array();
	}

	void endArray() {
		SGE_ASSERT(_stack.back()->is_array());
	}

	void beginArrayElement() {
		auto& arr = _stack.back();
		SGE_ASSERT(arr->is_array());
		auto& elementValue = arr->emplace_back();
		_stack.emplace_back(&elementValue);
	}

	void endArrayElement() {
		_stack.pop_back();
		SGE_ASSERT(_stack.back()->is_array());
	}

private:
	Json&	_out;
	Vector_<Json*, 64>	_stack;
};

}
