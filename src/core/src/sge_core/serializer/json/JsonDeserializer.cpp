#include "JsonDeserializer.h"

namespace sge {

JsonDeserializer::JsonDeserializer(StrView json) {
	_json = Json::parse(json.begin(), json.end());
}

}

