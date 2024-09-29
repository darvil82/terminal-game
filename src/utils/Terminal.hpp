#pragma once

#include <cstdint>
#include <string>

#include "Color.hpp"
#include "Point.hpp"

namespace utils {
	struct Terminal final {
		Terminal() = delete;

		static constexpr char ESCAPE = '\e';

		// text formatting
		static constexpr const char* INVERT = "[7m";
		static constexpr const char* NO_INVERT = "[27m";
		static constexpr const char* UNDERLINE = "[4m";
		static constexpr const char* NO_UNDERLINE = "[24m";
		static constexpr const char* DIM = "[2m";
		static constexpr const char* NO_DIM = "[22m";
		static constexpr const char* STRIKETHROUGH = "[9m";
		static constexpr const char* NO_STRIKETHROUGH = "[29m";
		static constexpr const char* INVISIBLE = "[8m";
		static constexpr const char* NO_INVISIBLE = "[28m";
		static constexpr const char* BOLD = "[1m";
		static constexpr const char* NO_BOLD = "[22m";
		static constexpr const char* ITALIC = "[3m";
		static constexpr const char* NO_ITALIC = "[23m";
		static constexpr const char* BLINK = "[5m";
		static constexpr const char* NO_BLINK = "[25m";
		static constexpr const char* RESET = "[0m";

		// special
		static constexpr const char* CLEAR_LINE = "[2K";
		static constexpr const char* CLEAR_RIGHT = "[0K";
		static constexpr const char* CLEAR_LEFT = "[1K";
		static constexpr const char* CLEAR_DOWN = "[0J";
		static constexpr const char* CLEAR_ALL = "[2J";
		static constexpr const char* CLEAR_SCROLL = "[3J";
		static constexpr const char* CURSOR_SHOW = "[?25h";
		static constexpr const char* CURSOR_HIDE = "[?25l";
		static constexpr const char* CURSOR_SAVE = "7";
		static constexpr const char* CURSOR_LOAD = "8";
		static constexpr const char* BUFFER_NEW = "[?1049h";
		static constexpr const char* BUFFER_OLD = "[?1049l";
		static constexpr const char* CURSOR_HOME = "[H";
		static constexpr const char* INVERT_ALL = "[?5h";
		static constexpr const char* NO_INVERT_ALL = "[?5l";

		static utils::UPoint get_terminal_size();
		static bool is_a_terminal();
		static bool is_a_tty();
		static std::string cursor_set_pos(utils::UPoint pos);
		static std::string cursor_set_pos_relative(utils::SPoint offset);
		static std::string cursor_move_x(int16_t x);
		static std::string cursor_move_y(int16_t y);
		static std::string set_color(const utils::Color color, bool background = false);
		static std::string prepare(bool alternate_buffer = false);
		static std::string restore(bool alternate_buffer = false);
	};
}