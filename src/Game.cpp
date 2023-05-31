#include <cwchar>
#include <thread>
#include "Game.hpp"
#include "utils/Terminal.hpp"
#include "utils/Cleanup.hpp"
#include "utils/Typedefs.hpp"
#include "entities/definition/EntityDB.hpp"
#include "entities/Cube.hpp"
#include "entities/bases/BaseEntity.hpp"
#include "input/InputSystem.hpp"


Game::Game() {
	auto [width, height] = utils::Terminal::get_terminal_size();
	this->renderer = std::make_unique<render::Renderer>(width, height);

	this->input_system = &input::InputSystem::instance();
	std::srand(std::time(nullptr));
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

//	for (int i = 0; i < 15; i++) {
//		s->attach_entity(ENTITY_CREATE(entities::Cube, cube));
//	}

	this->current_scene = s;

	this->renderer->start_render_loop([this](auto& render_utils) {
		this->render(render_utils);
	});
}

void Game::end() {
}

void Game::main_loop() {
	timestamp last_tick_time = chrono::steady_clock::now();
	constexpr const uint8_t max_tps = 50;

	while (this->running) {
		const timestamp current_tick_time = chrono::steady_clock::now();

		float delta = duration_cast<chrono::duration<float>>(current_tick_time - last_tick_time).count();
		last_tick_time = current_tick_time;

		this->tick(delta);
		this->input_system->reset_key_buff();

		// cap the max_tps
		if (delta < 1.0f / max_tps) {
			std::this_thread::sleep_for(chrono::duration<float>(1.0f / max_tps - delta));
		}
	}

	this->end();
}


void Game::tick(float delta) {
	if (this->current_scene)
		this->current_scene->tick(delta);

	if (INPUT_IS_PRESSED(SPACE)) {
		for (auto& cube: this->current_scene->get_entities_filtered<entities::Cube>(
			entities::ent_is_classname("cube"))
		) {
			cube->jump();
		}
	}

	if (INPUT_IS_PRESSED(C)) {
		this->current_scene->attach_entity(ENTITY_CREATE(entities::Cube, cube));
	}
}

void Game::render(render::Renderer& r) const {
	r << render::render_helpers::TextRenderHelper {{50, 10}}
		.set_alignment(render::render_helpers::Alignment::CENTER)
		.put_line(L"Press C to spawn a cube, SPACE to shake all cubes!");

	if (this->current_scene) {
		this->current_scene->render(r);
	}

	r << render::render_helpers::TextRenderHelper {{0, 0}}
		.put_line(L"ENTITIES: " + std::to_wstring(this->current_scene->get_entity_count()))
		.put_line(L"FPS: " + std::to_wstring(r.get_current_fps()));
}