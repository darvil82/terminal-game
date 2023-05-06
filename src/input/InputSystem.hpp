#pragma once

#include <unistd.h>
#include <termios.h>
#include <thread>
#include "../Game.hpp"
#include "Keyboard.hpp"

#define INPUT_IS_PRESSED(key) input::InputSystem::instance().is_key_pressed(INPUT_KEY(key))

class Game;

namespace input {
	class InputSystem {
		friend Game;
		using key_buff_t = std::array<char, 2>;

		termios old_terminal_config;
		uint32_t old_stdin_flags;

		key_buff_t pressed_key_buff;

		InputSystem();

		void read_input();
		void reset_key_buff();

	public:
		~InputSystem();
		bool is_key_pressed(keys::KeyboardKey key) const;

		static InputSystem& instance();
	};
}
