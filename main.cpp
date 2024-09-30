#include <iostream>
#include <csignal>

#include "src/Game.hpp"
#include "src/utils/Terminal.hpp"
#include "src/render/Character.hpp"


Game game;

int main() {
	if (!utils::Terminal::is_a_terminal())
		return 1;

	std::signal(SIGINT, [](int) {
		game.stop_loop();
	});

	game.start_loop();
}