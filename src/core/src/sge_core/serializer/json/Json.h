#pragma once

#include <nlohmann/json.hpp>
#include <sge_core/base/Error.h>

namespace sge {
	struct Json {
		Json() = default;
		Json(StrView json) {
			parse(json);
		}

		void parse(StrView json) {
			_obj = nlohmann::json::parse(json.begin(), json.end());
		}

	private:
		nlohmann::json	_obj;
	};
	
}
