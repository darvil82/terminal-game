#ifdef _WIN32
#include <conio.h>
#endif

#include <unistd.h>
#include <iostream>
#include <fstream>
#include "InputSystem.hpp"

#include "../utils/Terminal.hpp"
#include "../utils/Threads.hpp"


namespace input {
	InputSystem::InputSystem() {
		utils::Terminal::set_canonical_mode(true);

		// start input thread
		this->input_thread = utils::Thread(&InputSystem::read_input, this);
		this->input_thread.set_name("InputSystem");
	}

	InputSystem::~InputSystem() {
		utils::Terminal::set_canonical_mode(false);

		// stop input thread
		this->is_reading = false;
		this->input_thread.join();
	}

	void InputSystem::read_input() {
		while (this->is_reading) {
			key_buff_t buff = {0}; // clear current_buffer

			if (
#ifdef _WIN32
				kbhit() && // check on windows first if anything was pressed to prevent lockups
#endif
				read(0, &buff, buff.size()) > 0
			)
				this->pressed_key_buff = buff; // only if we read something
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