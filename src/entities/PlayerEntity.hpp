#pragma once

#include <iostream>
#include "BaseEntity.hpp"
#include "../utils/Point.hpp"
#include "EntityFactory.hpp"

namespace entities {

	class PlayerEntity : public BaseEntity {
	DEFINE_ENTITY_CLASS(player)

		std::string player_name;
		render::Color player_color = render::default_colors::WHITE;

	public:
		PlayerEntity() {
			this->position = {5, 5};
			this->velocity = {20, 8};
		}

		void render(render::Renderer& renderer) const override {
			renderer.set_pixel({
				render::default_characters::blocks::FULL,
				this->player_color
			}, this->position);
		}

		void tick(float delta) override {
			this->position += this->velocity * delta;

			bool hit_x =
				(this->position.x >= 90 && this->velocity.x > 0) || (this->position.x <= 0 && this->velocity.x < 0);
			bool hit_y =
				(this->position.y >= 25 && this->velocity.y > 0) || (this->position.y <= 0 && this->velocity.y < 0);

			if (hit_x)
				this->velocity.x *= -1;
			if (hit_y)
				this->velocity.y *= -1;

			if (hit_x || hit_y) {
				this->player_color = render::Color {
					static_cast<uint8_t>(rand() % 255),
					static_cast<uint8_t>(rand() % 255),
					static_cast<uint8_t>(rand() % 255)
				};
			}
		}
	};

	DEFINE_ENTITY_FACTORY(PlayerEntity)
}