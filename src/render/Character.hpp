#pragma once

namespace render {
	class UTF8Char {
		static constexpr const size_t char_size = 4;

		char character[char_size];

	};


	namespace default_characters {
		static const char* SPACE = " ";

		namespace lines {
			namespace light {
				static const char* HORIZONTAL = "─";
				static const char* VERTICAL = "│";
				static const char* TOP_RIGHT = "┐";
				static const char* TOP_LEFT = "┌";
				static const char* BOTTOM_RIGHT = "┘";
				static const char* BOTTOM_LEFT = "└";
				static const char* VERTICAL_RIGHT = "┤";
				static const char* VERTICAL_LEFT = "├";
				static const char* HORIZONTAL_DOWN = "┴";
				static const char* HORIZONTAL_UP = "┬";
				static const char* CROSS = "┼";
			}

			namespace bold {
				static const char* HORIZONTAL = "━";
				static const char* VERTICAL = "┃";
				static const char* TOP_RIGHT = "┓";
				static const char* TOP_LEFT = "┏";
				static const char* BOTTOM_RIGHT = "┛";
				static const char* BOTTOM_LEFT = "┗";
				static const char* VERTICAL_RIGHT = "┫";
				static const char* VERTICAL_LEFT = "┣";
				static const char* HORIZONTAL_DOWN = "┻";
				static const char* HORIZONTAL_UP = "┳";
				static const char* CROSS = "╋";
			}
		};

		namespace blocks {
			static const char* FULL = "█";
			static const char* FULL_1 = "▓";
			static const char* FULL_2 = "▒";
			static const char* FULL_3 = "░";
		};
	}
}
