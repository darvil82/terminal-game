#include <sys/ioctl.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <cstring>
#include <filesystem>

#include "Terminal.hpp"
#include "../render/Pixel.hpp"

namespace utils {

	utils::UPoint Terminal::get_terminal_size() {
		winsize w;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

		return {w.ws_col, w.ws_row};
	}

	bool Terminal::is_a_terminal() {
		return isatty(STDOUT_FILENO);
	}

	bool Terminal::is_a_tty() {
		if (!Terminal::is_a_terminal()) return false;

		// check if terminal name ends with "ttyN" where N is a number
		if (const char* terminal_name = ttyname(STDOUT_FILENO)) {
			return std::filesystem::path { terminal_name }
				.filename()
				.string()
				.starts_with("tty");
		}

		return false;
	}

	std::string Terminal::cursor_set_pos(utils::UPoint pos) {
		std::stringstream buff;
		buff << Terminal::ESCAPE_SEQUENCE_START
			<< pos.y + 1 << ';'
			<< pos.x + 1 << 'f';
		return buff.str();
	}

	std::string Terminal::cursor_move_x(int16_t x) {
		if (x == 0) return "";

		std::stringstream buff;
		buff << Terminal::ESCAPE_SEQUENCE_START;

		if (auto absolute = abs(x); absolute != 1)
			buff << absolute;

		buff << (x < 0 ? 'D' : 'C');
		return buff.str();
	}

	std::string Terminal::cursor_move_y(int16_t y) {
		if (y == 0) return "";

		std::stringstream buff;
		buff << Terminal::ESCAPE_SEQUENCE_START;

		if (auto absolute = abs(y); absolute != 1)
			buff << absolute;

		buff << (y < 0 ? 'A' : 'B');
		return buff.str();
	}

	std::string Terminal::cursor_set_pos_relative(utils::SPoint offset) {
		return Terminal::cursor_move_x(offset.x)
			+ Terminal::cursor_move_y(offset.y);
	}

	std::string Terminal::set_color(const Color color, bool background) {
		std::stringstream buff;
		buff << Terminal::ESCAPE_SEQUENCE_START
			<< (background ? "48" : "38") << ";2;"
			<< color.r << ';'
			<< color.g << ';'
			<< color.b << 'm';
		return buff.str();
	}
}