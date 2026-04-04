#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <sys/ioctl.h>
#include <termios.h>
#include <fcntl.h>
#endif

#include <unistd.h>
#include <sstream>
#include <iostream>
#include <filesystem>

#include "Terminal.hpp"
#include "../render/Pixel.hpp"

namespace utils {

	utils::UPoint Terminal::get_terminal_size() {
#ifdef _WIN32
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
		return {
			static_cast<uint32_t>(info.srWindow.Right - info.srWindow.Left + 1),
			static_cast<uint32_t>(info.srWindow.Bottom - info.srWindow.Top + 1)
		};
#else
		winsize w;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
		return {w.ws_col, w.ws_row};
#endif
	}

	bool Terminal::is_a_terminal() {
		return isatty(STDOUT_FILENO);
	}

	bool Terminal::is_a_tty() {
#ifndef _WIN32
		if (!Terminal::is_a_terminal()) return false;

		if (const char* terminal_name = ttyname(STDOUT_FILENO)) {
			return std::filesystem::path { terminal_name }
				.filename()
				.string()
				.starts_with("tty");
		}

#endif
		return false;
	}

	void Terminal::set_canonical_mode(bool enabled) {
#ifdef _WIN32
		DWORD mode;
		auto handle = GetStdHandle(STD_INPUT_HANDLE);
		GetConsoleMode(handle, &mode);

		const auto flags = ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT;

		// Disable line input (canonical) and echo
		if (enabled) mode &= ~flags;
		else mode |= flags;

		SetConsoleMode(handle, mode);
#else
		static termios old_terminal_config;
		static uint32_t old_stdin_flags;

		if (enabled) {
			tcgetattr(STDIN_FILENO, &old_terminal_config);
			auto new_terminal_config = old_terminal_config; // copy old terminal config
			new_terminal_config.c_lflag &= ~ICANON; // disable canonical mode
			new_terminal_config.c_lflag &= ~ECHO; // disable echo

			tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal_config);

			old_stdin_flags = fcntl(STDIN_FILENO, F_GETFL, 0);
			fcntl(STDIN_FILENO, F_SETFL, old_stdin_flags | O_NONBLOCK); // set stdin to non-blocking
		} else {
			// restore old configs
			tcsetattr(STDIN_FILENO, TCSANOW, &old_terminal_config);
			fcntl(STDIN_FILENO, F_SETFL, old_stdin_flags);
		}
#endif
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
		auto absolute = abs(x);
		auto is_neg = x < 0;

		std::stringstream buff;

		if (is_neg && absolute < 4) {
			for (uint8_t x = 0; x < absolute; x++) {
				buff << '\b';
			}

			return buff.str();
		}

		buff << Terminal::ESCAPE << '[';

		if (absolute != 1)
			buff << absolute;

		buff << (is_neg ? 'D' : 'C');
		return buff.str();
	}

	std::string Terminal::cursor_move_y(int16_t y) {
		if (y == 0) return "";
		auto absolute = abs(y);
		auto is_neg = y < 0;

		std::stringstream buff;

		if (absolute < 4 && !is_neg) {
			for (uint8_t x = 0; x < absolute; x++) {
				buff << '\v';
			}

			return buff.str();
		}

		buff << Terminal::ESCAPE << '[';

		if (absolute != 1)
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

	std::string Terminal::clear(bool all) {
		std::stringstream buff;

		buff << Terminal::ESCAPE;

		if (all)
			buff << Terminal::CURSOR_HOME << Terminal::ESCAPE;

		buff << Terminal::CLEAR_ALL;

		return buff.str();
	}

	std::string Terminal::prepare(bool alternate_buffer) {
		std::stringstream buff;

		buff << Terminal::ESCAPE << Terminal::CURSOR_HIDE;

		if (alternate_buffer) buff << Terminal::ESCAPE << Terminal::BUFFER_NEW;
		else buff << Terminal::clear(true);

#ifdef _WIN32
		SetConsoleOutputCP(CP_UTF8); // make cmd output utf8 characters
#endif

		return buff.str();
	}

	std::string Terminal::restore(bool alternate_buffer) {
		std::stringstream buff;

		buff << Terminal::ESCAPE << Terminal::CURSOR_SHOW;

		if (alternate_buffer) buff << Terminal::ESCAPE << Terminal::BUFFER_OLD;
		else buff << Terminal::clear(true);

		return buff.str();
	}
}