#include <iostream>
#include <csignal>
#include <thread>

#include "src/Game.hpp"
#include "src/entities/bases/BaseHealthEntity.hpp"
#include "src/utils/Cleanup.hpp"


Game game;

int main() {
	std::signal(SIGINT, [](int) {
		game.stop_loop();
	});

	game.start_loop();
}