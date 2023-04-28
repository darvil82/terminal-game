export module game;

import <memory>;
import <string>;
import <thread>;
import <chrono>;
import <locale>;

import game.scene;

namespace chrono = std::chrono;
using timestamp = decltype(chrono::steady_clock::now());

export class Game {
	std::unique_ptr<render::Renderer> renderer;
	Scene* current_scene = nullptr;
	bool running = false;

	void tick(float delta) {
		if (this->current_scene)
			this->current_scene->tick(delta);
	}
	void render() {
		this->renderer->clear_buffer();

		if (this->current_scene) {
			this->current_scene->render(this->renderer->get_render_utils());
		}

		this->renderer->push_buffer();
	}
	void main_loop() {
		timestamp last_frame = chrono::steady_clock::now();
		const uint8_t max_fps = 60;

		while (this->running) {
			const timestamp current_frame = chrono::steady_clock::now();

			float delta = duration_cast<chrono::duration<float>>(current_frame - last_frame).count();
			last_frame = current_frame;

			this->tick(delta);
			this->render();

			if (delta < 1.0f / max_fps) {
				std::this_thread::sleep_for(chrono::duration<float>(1.0f / max_fps - delta));
			}
		}

		this->end();
	}
	void init() {
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
	void end() {
		this->renderer->end();
	}

public:
	Game() : renderer {std::make_unique<render::Renderer>(90, 25)} { }

	void start_loop() {
		if (this->running) return;
		this->running = true;

		this->init();
		this->main_loop();
	}
	void stop_loop() {
		if (!this->running) return;
		this->running = false;
	}
};
