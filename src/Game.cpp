#include "Game.hpp"
#include <cwchar>
#include <thread>

namespace chrono = std::chrono;
using timestamp = decltype(chrono::steady_clock::now());

void Game::start() {
	this->main_loop();
}

void Game::main_loop() {
	timestamp last_frame = chrono::steady_clock::now();

	while (true) {
		const timestamp current_frame = chrono::steady_clock::now();

		float delta = duration_cast<chrono::duration<float>>(current_frame - last_frame).count();
		last_frame = current_frame;

		this->tick(delta);
		this->render();
	}
}


void Game::tick(float delta) { }

void Game::render() {
	if (this->current_scene)
		this->current_scene->render(*this->renderer);
}