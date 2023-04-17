#pragma once

#include <chrono>
#include <memory>
#include "render/Renderable.hpp"
#include "Tickable.hpp"
#include "Scene.hpp"


class Game {
	std::unique_ptr<render::Renderer> renderer;
	Scene* current_scene = nullptr;
	bool running = false;

	void tick(float delta);
	void render();
	void main_loop();

public:
	Game() {
		this->renderer = std::make_unique<render::Renderer>(40, 20);
	}

	void start();
	void stop();
};
