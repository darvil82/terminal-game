#pragma once

#include <iostream>
#include "BaseEntity.hpp"
#include "../utils/Point.hpp"

namespace entities {

	class Cube : public BaseEntity {
		utils::Color color = utils::default_colors::WHITE;

	protected:
		void render(render::Renderer& renderer) const override;
		void tick(float delta) override;

	public:
		Cube() {
			this->position = {static_cast<float>(rand() % 90), static_cast<float>(rand() % 24)};
			this->color = utils::Color {static_cast<uint8_t>(rand() % 255), static_cast<uint8_t>(rand() % 255),
				static_cast<uint8_t>(rand() % 255)};
			this->velocity = {static_cast<float>(rand() % 100 - 50), static_cast<float>(rand() % 100 - 50)};
		}

		void jump();
	};
}
