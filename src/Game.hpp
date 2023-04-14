#pragma once

#include <chrono>
#include "render/Renderable.hpp"
#include "Tickable.hpp"
#include "Scene.hpp"


class Game {
	render::Renderer& renderer;
	Scene* current_scene = nullptr;

	void tick(float delta);
	void render();
	void main_loop();

public:
	Game(render::Renderer& renderer) : renderer{renderer} { }

	void start();
};
