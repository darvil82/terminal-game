#pragma once

#include <iostream>
#include "BaseEntity.hpp"
#include "../utils/Point.hpp"
#include "HealthEntity.hpp"

namespace entities {

	class PlayerEntity : public HealthEntity {
	DEFINE_ENTITY_CLASS(player)

		std::string player_name;
		render::Color player_color = render::default_colors::WHITE;

	public:
		PlayerEntity() {
			this->position = {5, 5};
			this->velocity = {20, 8};
		}

		void render(render::render_helpers::RenderUtils&& renderer) const override;
		void tick(float delta) override;
	};


	DEFINE_ENTITY_FACTORY(PlayerEntity)
}