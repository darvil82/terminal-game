#pragma once

namespace render {
	namespace default_characters {
		const std::string SPACE = " ";

		namespace lines {
			namespace light {
				const std::string HORIZONTAL = "─";
				const std::string VERTICAL = "│";
				const std::string TOP_RIGHT = "┐";
				const std::string TOP_LEFT = "┌";
				const std::string BOTTOM_RIGHT = "┘";
				const std::string BOTTOM_LEFT = "└";
				const std::string VERTICAL_RIGHT = "┤";
				const std::string VERTICAL_LEFT = "├";
				const std::string HORIZONTAL_DOWN = "┴";
				const std::string HORIZONTAL_UP = "┬";
				const std::string CROSS = "┼";
			}

			namespace bold {
				const std::string HORIZONTAL = "━";
				const std::string VERTICAL = "┃";
				const std::string TOP_RIGHT = "┓";
				const std::string TOP_LEFT = "┏";
				const std::string BOTTOM_RIGHT = "┛";
				const std::string BOTTOM_LEFT = "┗";
				const std::string VERTICAL_RIGHT = "┫";
				const std::string VERTICAL_LEFT = "┣";
				const std::string HORIZONTAL_DOWN = "┻";
				const std::string HORIZONTAL_UP = "┳";
				const std::string CROSS = "╋";
			}
		};

		namespace blocks {
			const std::string FULL = "█";
			const std::string FULL_1 = "▓";
			const std::string FULL_2 = "▒";
			const std::string FULL_3 = "░";
		};
	}
}
