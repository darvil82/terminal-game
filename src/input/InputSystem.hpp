#pragma once

#include <thread>
#include "Keyboard.hpp"
#include "InputTypes.hpp"
#include "../Game.hpp"
#include "../utils/Threads.hpp"

#define INPUT_IS_PRESSED(key) input::InputSystem::instance().is_key_pressed(INPUT_KEY(key))


class Game;

namespace input {

	class InputSystem {
		friend Game;

		utils::Thread input_thread;
		bool is_reading = true;

		key_buff_t pressed_key_buff;
		// used to check if the key current_buffer has changed since the last reset
		key_buff_t last_pressed_key_buff;

		InputSystem();

		void read_input();
		void reset_key_buff();

	public:
		~InputSystem();
		bool is_key_pressed(const keys::KeyboardKey& key) const;

		static InputSystem& instance();
	};
}
