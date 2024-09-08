#pragma once

#include <sstream>


namespace render {
	struct UTF8Char {
		uint32_t codepoint;

		constexpr UTF8Char(const uint32_t character): codepoint{character} {}

		std::string to_string() const;

		constexpr bool operator==(UTF8Char other) const {
			return codepoint == other.codepoint;
		}
	};


	namespace default_characters {
		static constexpr UTF8Char SPACE = L' ';

		namespace lines {
			namespace light {
				static constexpr UTF8Char HORIZONTAL = L'─';
				static constexpr UTF8Char VERTICAL = L'│';
				static constexpr UTF8Char TOP_RIGHT = L'┐';
				static constexpr UTF8Char TOP_LEFT = L'┌';
				static constexpr UTF8Char BOTTOM_RIGHT = L'┘';
				static constexpr UTF8Char BOTTOM_LEFT = L'└';
				static constexpr UTF8Char VERTICAL_RIGHT = L'┤';
				static constexpr UTF8Char VERTICAL_LEFT = L'├';
				static constexpr UTF8Char HORIZONTAL_DOWN = L'┴';
				static constexpr UTF8Char HORIZONTAL_UP = L'┬';
				static constexpr UTF8Char CROSS = L'┼';
			}

			namespace bold {
				static constexpr UTF8Char HORIZONTAL = L'━';
				static constexpr UTF8Char VERTICAL = L'┃';
				static constexpr UTF8Char TOP_RIGHT = L'┓';
				static constexpr UTF8Char TOP_LEFT = L'┏';
				static constexpr UTF8Char BOTTOM_RIGHT = L'┛';
				static constexpr UTF8Char BOTTOM_LEFT = L'┗';
				static constexpr UTF8Char VERTICAL_RIGHT = L'┫';
				static constexpr UTF8Char VERTICAL_LEFT = L'┣';
				static constexpr UTF8Char HORIZONTAL_DOWN = L'┻';
				static constexpr UTF8Char HORIZONTAL_UP = L'┳';
				static constexpr UTF8Char CROSS = L'╋';
			}
		};

		namespace blocks {
			static constexpr UTF8Char FULL = L'█';
			static constexpr UTF8Char FULL_1 = L'▓';
			static constexpr UTF8Char FULL_2 = L'▒';
			static constexpr UTF8Char FULL_3 = L'░';
		};
	}
}
