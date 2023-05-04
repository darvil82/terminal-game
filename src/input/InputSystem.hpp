#pragma once

#include <unistd.h>
#include <termios.h>
#include <thread>
#include "../Game.hpp"

#define INPUT_IS_PRESSED(key) input::InputSystem::instance().is_key_pressed(key)

class Game;

namespace input {
	class InputSystem {
		friend Game;
		static constexpr size_t key_buff_size = 8;

		std::thread input_reader;
		termios old_terminal_config;
		termios new_terminal_config;
		std::array<char, key_buff_size> pressed_key_buff;
		std::array<char, key_buff_size> last_key_buff;
		bool is_reading = true;

		InputSystem();

		void read_input();
		void reset_key_buff();

	public:
		~InputSystem();
		bool is_key_pressed(char key);

		static InputSystem& instance();
	};
}
