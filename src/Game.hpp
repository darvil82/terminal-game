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
	void init();
	void end();

public:
	Game(): renderer{std::make_unique<render::Renderer>(90, 25)} {}

	void start_loop();
	void stop_loop();
};
