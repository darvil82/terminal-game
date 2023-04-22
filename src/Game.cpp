#include "Game.hpp"
#include "utils/Cleanup.hpp"
#include "entities/EntityDB.hpp"
#include "entities/PlayerEntity.hpp"
#include "entities/BaseEntity.hpp"
#include <cwchar>

namespace chrono = std::chrono;
using timestamp = decltype(chrono::steady_clock::now());

void Game::start_loop() {
	if (this->running) return;
	this->running = true;

	this->init();
	this->main_loop();
}

void Game::stop_loop() {
	if (!this->running) return;
	this->running = false;
}

void Game::init() {
	std::string prev_loc = std::setlocale(LC_ALL, nullptr);
	std::setlocale(LC_ALL, "en_US.utf8");

	const auto cleanup = utils::Cleanup([&prev_loc] {
		std::setlocale(LC_ALL, prev_loc.c_str());
	});

	this->renderer->init();

	Scene* s = new Scene();
	auto& x = ENTITY_CREATE(entities::PlayerEntity, player);
	s->attach_entity(x);
	this->current_scene = s;
}

void Game::end() {
	this->renderer->end();
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

	this->end();
}


void Game::tick(float delta) {
	if (this->current_scene)
		this->current_scene->tick(delta);
}

void Game::render() {
	this->renderer->clear_buffer();

	if (this->current_scene)
		this->current_scene->render(*this->renderer);

	this->renderer->push_buffer();
}