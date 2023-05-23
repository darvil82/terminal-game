#pragma once

#include <iostream>
#include "bases/BaseEntity.hpp"
#include "../utils/Point.hpp"
#include "bases/BaseHealthEntity.hpp"

namespace entities {

	class Cube : public BaseHealthEntity {
	DEFINE_ENTITY_CLASS(cube)

		std::string player_name;
		utils::Color color = utils::default_colors::WHITE;

	public:
		Cube() {
			this->position = {static_cast<float>(rand() % 90), static_cast<float>(rand() % 24)};
			this->color = utils::Color {static_cast<uint8_t>(rand() % 255), static_cast<uint8_t>(rand() % 255),
				static_cast<uint8_t>(rand() % 255)};
			this->velocity = {static_cast<float>(rand() % 100 - 50), static_cast<float>(rand() % 100 - 50)};
		}

		void render(render::Renderer& renderer) const override;
		void tick(float delta) override;
		void jump();
	};


	DEFINE_ENTITY_FACTORY(Cube)
}
