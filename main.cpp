#include <iostream>

#include <typeinfo>
#include "src/render/Renderer.hpp"
#include "src/utils/Cleanup.hpp"
#include "src/Scene.hpp"
#include "src/entities/BaseEntity.hpp"
#include "src/entities/PlayerEntity.hpp"
#include "src/Game.hpp"
#include <csignal>


Game game;

int main() {
	std::signal(SIGINT, [](int) {
		game.stop_loop();
	});

	game.start_loop();
}

