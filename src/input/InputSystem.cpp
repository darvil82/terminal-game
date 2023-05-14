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

		// start input thread
		this->input_thread = std::thread(&InputSystem::read_input, this);
	}

	InputSystem::~InputSystem() {
		// restore old terminal config
		tcsetattr(STDIN_FILENO, TCSANOW, &this->old_terminal_config);

		// restore old stdin flags
		fcntl(STDIN_FILENO, F_SETFL, this->old_stdin_flags);

		// stop input thread
		this->is_reading = false;
		this->input_thread.join();
	}

	void InputSystem::read_input() {
		while (this->is_reading) {
			key_buff_t buff = {0}; // clear current_buffer
			if (read(0, &buff, buff.size()) > 0) {
				this->pressed_key_buff = buff; // only if we read something
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}

	/**
	 * Resets the key current_buffer if the key current_buffer has not changed since the last reset.
	 */
	void InputSystem::reset_key_buff() {
		if (this->pressed_key_buff == this->last_pressed_key_buff) {
			this->pressed_key_buff.fill(0);
		}

		this->last_pressed_key_buff = this->pressed_key_buff;
	}

	bool InputSystem::is_key_pressed(const keys::KeyboardKey& key) const {
		return key.matches(this->last_pressed_key_buff);
	}


	InputSystem& input::InputSystem::instance() {
		static InputSystem input_system;
		return input_system;
	}
}