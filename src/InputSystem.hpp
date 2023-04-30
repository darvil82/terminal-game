#pragma once

#include <unistd.h>
#include <termios.h>
#include <thread>
#include "Game.hpp"



class InputSystem {
	std::thread input_reader;
	termios old_terminal_config;
	termios new_terminal_config;
	char last_char;
	bool is_reading = true;

	InputSystem();

	void read_input();

public:
	~InputSystem();
	static InputSystem& instance();
};
