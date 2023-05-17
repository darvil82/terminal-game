#include <sys/ioctl.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <cstring>
#include "Terminal.hpp"
#include "../render/Pixel.hpp"

namespace render {

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

		if (const char* terminal_name = ttyname(STDOUT_FILENO)) {
			// check if terminal name ends with "ttyN" where N is a number
			for (auto i = strlen(terminal_name) - 1; i >= 0; i--) {
				if (terminal_name[i] == '/')
					break;
				if (terminal_name[i] == 'y' && terminal_name[i - 1] == 't' && terminal_name[i - 2] == 't')
					return true;
			}
		}

		return false;
	}

	std::wstring Terminal::cursor_set_pos(utils::UPoint pos) {
		std::wstringstream buff;
		buff << Terminal::ESCAPE_SEQUENCE_START
			<< pos.y + 1 << ';'
			<< pos.x + 1 << 'f';
		return buff.str();
	}

	std::wstring Terminal::cursor_move_x(int16_t x) {
		if (x == 0) return L"";

		std::wstringstream buff;
		buff << Terminal::ESCAPE_SEQUENCE_START;

		if (auto absolute = abs(x); absolute != 1)
			buff << absolute;

		buff << (x < 0 ? 'D' : 'C');
		return buff.str();
	}

	std::wstring Terminal::cursor_move_y(int16_t y) {
		if (y == 0) return L"";

		std::wstringstream buff;
		buff << Terminal::ESCAPE_SEQUENCE_START;

		if (auto absolute = abs(y); absolute != 1)
			buff << absolute;

		buff << (y < 0 ? 'A' : 'B');
		return buff.str();
	}

	std::wstring Terminal::cursor_set_pos_relative(utils::SPoint offset) {
		return Terminal::cursor_move_x(offset.x)
			+ Terminal::cursor_move_y(offset.y);
	}

	std::wstring Terminal::set_color(const Color color, bool background) {
		std::wstringstream buff;
		buff << Terminal::ESCAPE_SEQUENCE_START
			<< (background ? "48" : "38") << ";2;"
			<< color.r << ';'
			<< color.g << ';'
			<< color.b << 'm';
		return buff.str();
	}
}