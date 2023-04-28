import <iostream>;
import <csignal>;

import game;

Game game;

int main() {
	std::signal(SIGINT, [](int) {
		game.stop_loop();
	});

	game.start_loop();
}

