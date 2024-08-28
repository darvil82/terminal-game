#pragma once

#include <cstdint>
#include <string>

#include "Color.hpp"
#include "Point.hpp"

namespace utils {
	struct Terminal final {
		Terminal() = delete;

		static constexpr const char ESCAPE = '\x1b';
		static constexpr const char* ESCAPE_SEQUENCE_START = "\x1b[";

		// text formatting
		static constexpr const char* INVERT = "\x1b" "[7m";
		static constexpr const char* NO_INVERT = "\x1b" "[27m";
		static constexpr const char* UNDERLINE = "\x1b" "[4m";
		static constexpr const char* NO_UNDERLINE = "\x1b" "[24m";
		static constexpr const char* DIM = "\x1b" "[2m";
		static constexpr const char* NO_DIM = "\x1b" "[22m";
		static constexpr const char* STRIKETHROUGH = "\x1b" "[9m";
		static constexpr const char* NO_STRIKETHROUGH = "\x1b" "[29m";
		static constexpr const char* INVISIBLE = "\x1b" "[8m";
		static constexpr const char* NO_INVISIBLE = "\x1b" "[28m";
		static constexpr const char* BOLD = "\x1b" "[1m";
		static constexpr const char* NO_BOLD = "\x1b" "[22m";
		static constexpr const char* ITALIC = "\x1b" "[3m";
		static constexpr const char* NO_ITALIC = "\x1b" "[23m";
		static constexpr const char* BLINK = "\x1b" "[5m";
		static constexpr const char* NO_BLINK = "\x1b" "[25m";
		static constexpr const char* RESET = "\x1b" "[0m";

		// special
		static constexpr const char* CLEAR_LINE = "\x1b" "[2K";
		static constexpr const char* CLEAR_RIGHT = "\x1b" "[0K";
		static constexpr const char* CLEAR_LEFT = "\x1b" "[1K";
		static constexpr const char* CLEAR_DOWN = "\x1b" "[0J";
		static constexpr const char* CLEAR_ALL = "\x1b" "[2J";
		static constexpr const char* CLEAR_SCROLL = "\x1b" "[3J";
		static constexpr const char* CURSOR_SHOW = "\x1b" "[?25h";
		static constexpr const char* CURSOR_HIDE = "\x1b" "[?25l";
		static constexpr const char* CURSOR_SAVE = "\x1b" "7";
		static constexpr const char* CURSOR_LOAD = "\x1b" "8";
		static constexpr const char* BUFFER_NEW = "\x1b" "[?1049h";
		static constexpr const char* BUFFER_OLD = "\x1b" "[?1049l";
		static constexpr const char* CURSOR_HOME = "\x1b" "[H";
		static constexpr const char* INVERT_ALL = "\x1b" "[?5h";
		static constexpr const char* NO_INVERT_ALL = "\x1b" "[?5l";

		static utils::UPoint get_terminal_size();
		static bool is_a_terminal();
		static bool is_a_tty();
		static std::string cursor_set_pos(utils::UPoint pos);
		static std::string cursor_set_pos_relative(utils::SPoint offset);
		static std::string cursor_move_x(int16_t x);
		static std::string cursor_move_y(int16_t y);
		static std::string set_color(const utils::Color color, bool background = false);
	};
}