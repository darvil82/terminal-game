#include <iostream>
#include <csignal>
#include <thread>

#include "src/Game.hpp"
#include "src/entities/bases/BaseHealthEntity.hpp"
#include "src/utils/Cleanup.hpp"


Game game;

int main() {
	const char* prev_loc = std::setlocale(LC_ALL, nullptr);
	std::setlocale(LC_ALL, "en_US.utf8");

	// reset locale back on exit
	ON_CLEANUP({
		std::setlocale(LC_ALL, prev_loc);
	});



	std::signal(SIGINT, [](int) {
		game.stop_loop();
	});

	game.start_loop();
}

