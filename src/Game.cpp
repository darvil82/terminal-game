#include <cwchar>
#include <thread>
#include "Game.hpp"
#include "utils/Cleanup.hpp"
#include "entities/definition/EntityDB.hpp"
#include "entities/Cube.hpp"
#include "entities/bases/BaseEntity.hpp"
#include "input/InputSystem.hpp"

namespace chrono = std::chrono;
using timestamp = decltype(chrono::steady_clock::now());

Game::Game() {
	this->renderer = std::make_unique<render::Renderer>(90, 25);
	this->input_system = &input::InputSystem::instance();
}

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
	Scene* s = new Scene();
	for (int i = 0; i < 5; i++) {
		s->attach_entity(ENTITY_CREATE(entities::Cube, cube));
	}
	this->current_scene = s;
}

void Game::end() {
}

void Game::main_loop() {
	timestamp last_frame_time = chrono::steady_clock::now();
	constexpr const uint8_t max_fps = 60;

	while (this->running) {
		const timestamp current_frame_time = chrono::steady_clock::now();

		float delta = duration_cast<chrono::duration<float>>(current_frame_time - last_frame_time).count();
		last_frame_time = current_frame_time;

		this->tick(delta);
		this->render();
		this->input_system->reset_key_buff();

		// cap fps
		if (delta < 1.0f / max_fps) {
			std::this_thread::sleep_for(chrono::duration<float>(1.0f / max_fps - delta));
		}
	}

	this->end();
}


void Game::tick(float delta) {
	if (this->current_scene)
		this->current_scene->tick(delta);

	if (INPUT_IS_PRESSED(SPACE)) {
		for (auto& cube : this->current_scene->get_entities_filtered<entities::Cube>(
			entities::ent_is_classname("cube"))
		) {
			cube->jump();
		}
	}

	if (INPUT_IS_PRESSED(C)) {
		this->current_scene->attach_entity(ENTITY_CREATE(entities::Cube, cube));
	}
}

void Game::render() {
	this->renderer->clear_buffer();

	if (this->current_scene) {
		this->current_scene->render(this->renderer->get_render_utils());
	}

	this->renderer->get_render_utils().text({0, 0}, [this](auto&& op) {
		op.put_line(L"ENTITIES: " + std::to_wstring(this->current_scene->get_entity_count()));
 	});
	this->renderer->push_buffer();
}