#include "PlayerEntity.hpp"
#include "../input/InputSystem.hpp"
#include "../utils/Math.hpp"

namespace entities {

	void PlayerEntity::render(render::render_helpers::RenderUtils&& renderer) const {
		renderer.draw(this->position, [this](auto&& r) {
			r.set_color(this->player_color);
			r.move_x(4);
			r.move_y(3);
			r.move_x(-12);
			r.move_y(-3);
		});

		renderer.text(this->position, [this](auto&& r) {
			r.set_position_relative({0, -3});
			r.set_color(render::default_colors::BLACK);
			r.set_color_bg(render::default_colors::CYAN);
			r.put(std::to_wstring(this->position.x) + L", " + std::to_wstring(this->position.y));
		});
	}

	void PlayerEntity::tick(float delta) {
		BaseEntity::tick(delta);

		if (INPUT_IS_PRESSED('w')) this->velocity.y -= 2;
		if (INPUT_IS_PRESSED('s')) this->velocity.y += 2;
		if (INPUT_IS_PRESSED('a')) this->velocity.x -= 2;
		if (INPUT_IS_PRESSED('d')) this->velocity.x += 2;


		bool hit_x =
			(this->position.x >= 90 && this->velocity.x > 0) || (this->position.x <= 0 && this->velocity.x < 0);
		bool hit_y =
			(this->position.y >= 25 && this->velocity.y > 0) || (this->position.y <= 0 && this->velocity.y < 0);

		if (hit_x) this->velocity.x *= -1;
		if (hit_y) this->velocity.y *= -1;

		// slowly decrease velocity
		this->velocity *= 0.997;

		// apply gravity
		this->velocity.y += 0.25;

		if (this->position.y >= 25) {
			this->position.y = 24.99;
		}

		if (INPUT_IS_PRESSED(' ')) {
			this->player_color = render::Color {
				static_cast<uint8_t>(rand() % 255),
				static_cast<uint8_t>(rand() % 255),
				static_cast<uint8_t>(rand() % 255)
			};
		}
	}

} // entities