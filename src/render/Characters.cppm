export module render.characters;

namespace render {
	namespace default_characters {
		namespace lines {
			namespace light {
				export enum : wchar_t {
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
				export enum : wchar_t {
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
			export enum : wchar_t {
				FULL = L'█',
				THREE_QUARTERS = L'▓',
				HALF = L'▒',
				QUARTER = L'░',
			};
		};
	}
}
