#pragma once

namespace sge {

struct JsonDeserializer : public NonCopyable {
public:
	JsonDeserializer(StrView json);

	template<class D>
	void io(D& data) {
	}

private:
	Json _json;
};

}
