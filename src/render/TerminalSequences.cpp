#include <sys/ioctl.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include "TerminalSequences.hpp"
#include "Pixel.hpp"

namespace render {

	utils::UPoint TerminalSequences::get_terminal_size() {
		winsize w;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

		return {w.ws_col, w.ws_row};
	}

	std::wstring TerminalSequences::cursor_set_pos(utils::UPoint pos) {
		std::wstringstream buff;
		buff << TerminalSequences::ESCAPE_SEQUENCE_START
			<< pos.y << ';'
			<< pos.x << 'f';
		return buff.str();
	}

	std::wstring TerminalSequences::cursor_move_x(int16_t x) {
		if (x == 0) return L"";

		std::wstringstream buff;
		buff << TerminalSequences::ESCAPE_SEQUENCE_START
			<< abs(x)
			<< (x < 0 ? 'D' : 'C');
		return buff.str();
	}

	std::wstring TerminalSequences::cursor_move_y(int16_t y) {
		if (y == 0) return L"";

		std::wstringstream buff;
		buff << TerminalSequences::ESCAPE_SEQUENCE_START
			<< abs(y)
			<< (y < 0 ? 'A' : 'B');
		return buff.str();
	}

	std::wstring TerminalSequences::cursor_set_pos_relative(utils::SPoint offset) {
		return TerminalSequences::cursor_move_x(offset.x)
			+ TerminalSequences::cursor_move_y(offset.y);
	}

	std::wstring TerminalSequences::set_color(Color color, bool background) {
		std::wstringstream buff;
		buff << TerminalSequences::ESCAPE_SEQUENCE_START
			<< (background ? "48" : "38") << ";2;"
			<< color.r << ';'
			<< color.g << ';'
			<< color.b << 'm';
		return buff.str();
	}
}