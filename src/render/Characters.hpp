#pragma once

namespace render {
	namespace default_characters {
		enum : wchar_t {
			SPACE = L' ',
		};

		namespace lines {
			namespace light {
				enum : wchar_t {
					HORIZONTAL = L'─',
					VERTICAL = L'│',
					TOP_RIGHT = L'┐',
					TOP_LEFT = L'┌',
					BOTTOM_RIGHT = L'┘',
					BOTTOM_LEFT = L'└',
					VERTICAL_RIGHT = L'┤',
					VERTICAL_LEFT = L'├',
					HORIZONTAL_DOWN = L'┴',
					HORIZONTAL_UP = L'┬',
					CROSS = L'┼',
				};
			}

			namespace bold {
				enum : wchar_t {
					HORIZONTAL = L'━',
					VERTICAL = L'┃',
					TOP_RIGHT = L'┓',
					TOP_LEFT = L'┏',
					BOTTOM_RIGHT = L'┛',
					BOTTOM_LEFT = L'┗',
					VERTICAL_RIGHT = L'┫',
					VERTICAL_LEFT = L'┣',
					HORIZONTAL_DOWN = L'┻',
					HORIZONTAL_UP = L'┳',
					CROSS = L'╋',
				};
			}
		};

		namespace blocks {
			enum : wchar_t {
				FULL = L'█',
				THREE_QUARTERS = L'▓',
				HALF = L'▒',
				QUARTER = L'░',
			};
		};
	}
}
