#include "String.h"
#include "UtfUtil.h"

namespace sge {

void StringUtil::appendBinToHex(String& result, const u8* data, size_t dataSize) {	
	const char* hex = "0123456789ABCDEF";
	size_t lineCount = (dataSize + 15) / 16;


	for (size_t line = 0; line < lineCount; line++) {
		size_t lineStart = line * 16;
		for (size_t i = 0; i < 16; i++) {
			size_t index = lineStart + i;

			if (index < dataSize) {
				u8 v = data[index];
				result.push_back(hex[v >> 4]);
				result.push_back(hex[v & 0xF]);
			} else {
				result.append("  ");
			}
			result.push_back(' ');
		}

		result.append("    ");

		for (size_t i = 0; i < 16; i++) {
			size_t index = lineStart + i;

			if (index < dataSize) {
				u8 v = data[index];
				if (v < 32) {
					result.push_back(' '); // non-printable character
				} else {
					result.push_back(v);
				}
			} else {
				result.push_back(' ');
			}
//			result.push_back(' ');
		}

		result.push_back('\n');
	}
}

} // namespace
