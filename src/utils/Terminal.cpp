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
		buff << Terminal::ESCAPE << '['
			<< pos.y + 1 << ';'
			<< pos.x + 1 << 'f';
		return buff.str();
	}

	std::string Terminal::cursor_move_x(int16_t x) {
		if (x == 0) return "";

		std::stringstream buff;
		buff << Terminal::ESCAPE << '[';

		if (auto absolute = abs(x); absolute != 1)
			buff << absolute;

		buff << (x < 0 ? 'D' : 'C');
		return buff.str();
	}

	std::string Terminal::cursor_move_y(int16_t y) {
		if (y == 0) return "";

		std::stringstream buff;
		buff << Terminal::ESCAPE << '[';

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
		buff << Terminal::ESCAPE << '['
			<< (background ? "48" : "38") << ";2;"
			<< std::to_string(color.r) << ';'
			<< std::to_string(color.g) << ';'
			<< std::to_string(color.b) << 'm';
		return buff.str();
	}

	std::string Terminal::init_new_buff() {
		std::stringstream buff;
		buff << Terminal::ESCAPE << Terminal::BUFFER_NEW
			<< Terminal::ESCAPE << Terminal::CURSOR_HIDE;
		return buff.str();
	}

	std::string Terminal::close_new_buff() {
		std::stringstream buff;
		buff << Terminal::ESCAPE << Terminal::CURSOR_SHOW
			<< Terminal::ESCAPE << Terminal::CLEAR_ALL
			<< Terminal::ESCAPE << Terminal::BUFFER_OLD;
		return buff.str();
	}
}