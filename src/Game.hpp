#pragma once

#include "render/Renderable.hpp"
#include "Tickable.hpp"


class Game : public ITickable, public render::IRenderable {
	void tick(float delta) override;
	void render(render::Renderer &renderer) override;

	void main_loop();

public:
	void start();
};
