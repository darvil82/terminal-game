#pragma once

#include <unistd.h>
#include <termios.h>
#include <thread>
#include "../Game.hpp"
#include "Keyboard.hpp"
#include "InputTypes.hpp"

#define INPUT_IS_PRESSED(key) input::InputSystem::instance().is_key_pressed(INPUT_KEY(key))


class Game;

namespace input {

	class InputSystem {
		friend Game;

		termios old_terminal_config;
		uint32_t old_stdin_flags;

		std::thread input_thread;
		bool is_reading = true;
		key_buff_t pressed_key_buff;
		key_buff_t last_pressed_key_buff; //

		InputSystem();

		void read_input();
		void reset_key_buff();

	public:
		~InputSystem();
		bool is_key_pressed(const keys::KeyboardKey& key) const;

		static InputSystem& instance();
	};
}
