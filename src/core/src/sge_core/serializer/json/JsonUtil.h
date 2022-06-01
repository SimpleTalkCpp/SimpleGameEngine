#pragma once

#include <sge_core/base/Error.h>
#include "JsonSerializer.h"
#include "JsonDeserializer.h"

namespace sge {

struct JsonUtil {
	JsonUtil() = delete;

	template<class T> static void readFile(StrView filename, T& obj);
	template<class T> static void writeFile(StrView filename, T& obj, bool createDir, bool logResult = true);
	template<class T> static void writeFileIfChanged(StrView filename, T& obj, bool createDir, bool logResult = true);

	template<class T> static void serialize  (String& json, T& obj);
	template<class T> static void deserialize(StrView json, T& obj);

	template<class T> static void stringify	(String& outJson, const T& obj);
};


template<class T> inline
void JsonUtil::readFile(StrView filename, T& obj) {
	String json;
	File::readFile(filename, json);
	JsonDeserializer de(json);
	de.io(obj);
}

template<class T> inline
void JsonUtil::writeFile(StrView filename, T& obj, bool createDir, bool logResult) {
	String json;
	serialize(json, obj);
	File::writeFile(filename, json, createDir, logResult);
}

template<class T> inline
void JsonUtil::writeFileIfChanged(StrView filename, T& obj, bool createDir, bool logResult) {
	String json;
	serialize(json, obj);
	File::writeFileIfChanged(filename, json, createDir, logResult);
}

template<class T> inline
void JsonUtil::serialize(String& outJson, T& obj) {
	JsonSerializer se(outJson);
	se.io(obj);
}

template<class T> inline
void JsonUtil::deserialize(StrView json, T& obj) {
	JsonDeserializer se(json);
	se.io(obj);
}

template<class T> inline
void JsonUtil::stringify(String& json, const T& obj) {
	JsonWriter wr(json);
	wr.writeValue(obj);
}


}