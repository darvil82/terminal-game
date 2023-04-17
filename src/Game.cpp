#include "Game.hpp"
#include "utils/Cleanup.hpp"
#include <cwchar>
#include <thread>

namespace chrono = std::chrono;
using timestamp = decltype(chrono::steady_clock::now());

void Game::start() {
	if (this->running) return;
	this->running = true;

	std::string prev_loc = std::setlocale(LC_ALL, nullptr);
	std::setlocale(LC_ALL, "en_US.utf8");

	const auto cleanup = utils::Cleanup([&prev_loc] {
		std::setlocale(LC_ALL, prev_loc.c_str());
	});

	this->renderer->start();

	this->main_loop();
}

void Game::stop() {
	this->running = false;
}

void Game::main_loop() {
	timestamp last_frame = chrono::steady_clock::now();

	while (this->running) {
		const timestamp current_frame = chrono::steady_clock::now();

		float delta = duration_cast<chrono::duration<float>>(current_frame - last_frame).count();
		last_frame = current_frame;

		this->tick(delta);
		this->render();
	}
}


void Game::tick(float delta) {
	if (this->current_scene)
		this->current_scene->tick(delta);
}

void Game::render() {
	if (this->current_scene)
		this->current_scene->render(*this->renderer);

	this->renderer->push_buffer();
}