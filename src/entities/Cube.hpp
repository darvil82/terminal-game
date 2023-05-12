#pragma once

#include <iostream>
#include "bases/BaseEntity.hpp"
#include "../utils/Point.hpp"
#include "bases/BaseHealthEntity.hpp"

namespace entities {

	class Cube : public BaseHealthEntity {
	DEFINE_ENTITY_CLASS(player)

		std::string player_name;
		render::Color player_color = render::default_colors::WHITE;
		uint8_t jumped = 2; // start with 2 jumps

	public:
		Cube() {
			this->position = {5, 5};
		}

		void render(const render::render_helpers::RenderUtils& render_utils) const override;
		void tick(float delta) override;
	};


	DEFINE_ENTITY_FACTORY(Cube)
}
