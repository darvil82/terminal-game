#include <math.h>
#include "Cube.hpp"
#include "../input/InputSystem.hpp"
#include "../utils/Math.hpp"

namespace entities {

	void Cube::render(render::Renderer& renderer) const {
		renderer << render::render_helpers::DrawRenderHelper { this->position }
			.set_color_fg(this->color)
			.set_thickness(4)
			.put();

		renderer << render::render_helpers::TextRenderHelper {utils::SPoint(this->position.x, this->position.y - 6)}
			.set_color_fg(utils::default_colors::CYAN)
			.set_alignment(render::render_helpers::Alignment::LEFT)
			.put_line("Vel: " + std::to_string(this->velocity.x) + " " + std::to_string(this->velocity.y))
			.put_line("Pos: " + std::to_string(this->position.x) + " " + std::to_string(this->position.y));
	}

	void Cube::tick(float delta) {
		BaseEntity::tick(delta);

		bool hit_x =
			(this->position.x >= 90 && this->velocity.x > 0) || (this->position.x <= 0 && this->velocity.x < 0);
		bool hit_y =
			(this->position.y >= 30 && this->velocity.y > 0) || (this->position.y <= 0 && this->velocity.y < 0);

		if (hit_x) this->velocity.x *= -1;
		if (hit_y) this->velocity.y *= -1;

		// slowly decrease velocity
		this->velocity *= std::pow(0.8, delta);

		// apply gravity
		this->velocity.y += 40 * delta;

		bool is_on_ground = this->position.y >= 30;

		if (is_on_ground) {
			this->position.y = 30;
			this->velocity.x *= std::pow(0.005, delta);
		}

	}

	void Cube::jump() {
		this->velocity.x = rand() % 100 - 50;
		this->velocity.y = rand() % 100 - 50;
	}

} // entities