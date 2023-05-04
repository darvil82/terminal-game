#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <fstream>
#include "InputSystem.hpp"


namespace input {
	InputSystem::InputSystem() {
		this->input_reader = std::thread {&InputSystem::read_input, this};

		tcgetattr(0, &old_terminal_config);
		this->new_terminal_config = old_terminal_config;

		auto* flags = &this->new_terminal_config.c_lflag;

		*flags &= ~ICANON; // disable canonical mode
		*flags &= ~ECHO; // disable echo

		tcsetattr(0, TCSANOW, &this->new_terminal_config);
	}

	InputSystem::~InputSystem() {
		this->is_reading = false;
		this->input_reader.join();

		// restore old terminal config
		tcsetattr(0, TCSANOW, &this->old_terminal_config);
	}

	void InputSystem::reset_key_buff() {
		if (this->pressed_key_buff == this->last_key_buff) {
			this->pressed_key_buff.fill(0);
		}

		this->last_key_buff = this->pressed_key_buff;
	}

	void InputSystem::read_input() {
		while (this->is_reading) {
			read(0, &this->pressed_key_buff, key_buff_size);
		}
	}

	bool InputSystem::is_key_pressed(char key) {
		return this->last_key_buff[0] == key;
	}



	InputSystem& input::InputSystem::instance() {
		static InputSystem input_system;
		return input_system;
	}
}