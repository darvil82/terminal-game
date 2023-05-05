#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include "InputSystem.hpp"


namespace input {
	InputSystem::InputSystem() {
		// set terminal to non-canonical mode and disable echo
		tcgetattr(0, &this->old_terminal_config);
		auto terminal_flags = this->old_terminal_config; // copy old terminal config

		terminal_flags.c_lflag &= ~ICANON; // disable canonical mode
		terminal_flags.c_lflag &= ~ECHO; // disable echo

		tcsetattr(STDIN_FILENO, TCSANOW, &terminal_flags);


		// set stdin to non-blocking
		this->old_stdin_flags = fcntl(STDIN_FILENO, F_GETFL, 0);
		fcntl(STDIN_FILENO, F_SETFL, this->old_stdin_flags | O_NONBLOCK);
	}

	InputSystem::~InputSystem() {
		// restore old terminal config
		tcsetattr(STDIN_FILENO, TCSANOW, &this->old_terminal_config);

		// restore old stdin flags
		fcntl(STDIN_FILENO, F_SETFL, this->old_stdin_flags);
	}

	void InputSystem::read_input() {
		if (read(0, &this->pressed_key_buff, this->pressed_key_buff.size()) > 0)
			return;

		// no input was read, reset key buffer
		if (this->pressed_key_buff[0] != '\0')
			this->pressed_key_buff.fill('\0');
	}

	bool InputSystem::is_key_pressed(char key) {
		return this->pressed_key_buff[0] == key;
	}



	InputSystem& input::InputSystem::instance() {
		static InputSystem input_system;
		return input_system;
	}
}