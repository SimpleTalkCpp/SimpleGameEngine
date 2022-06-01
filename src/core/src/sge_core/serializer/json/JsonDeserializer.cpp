#include "JsonDeserializer.h"

namespace sge {

JsonDeserializer::JsonDeserializer(StrView json) {
	_json.parse(json);
}

}

