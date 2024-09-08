#include "Character.hpp"

namespace render {
	std::string UTF8Char::to_string() const {
		std::string result;

		if (codepoint <= 0x7F) {
			// 1-byte UTF-8 (ASCII)
			result.push_back(static_cast<char>(codepoint));
		} else if (codepoint <= 0x7FF) {
			// 2-byte UTF-8
			result.push_back(static_cast<char>(0xC0 | ((codepoint >> 6) & 0x1F)));
			result.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
		} else if (codepoint <= 0xFFFF) {
			// 3-byte UTF-8
			result.push_back(static_cast<char>(0xE0 | ((codepoint >> 12) & 0x0F)));
			result.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
			result.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
		} else if (codepoint <= 0x10FFFF) {
			// 4-byte UTF-8
			result.push_back(static_cast<char>(0xF0 | ((codepoint >> 18) & 0x07)));
			result.push_back(static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F)));
			result.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
			result.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
		}

		return result;
	}
}