#include "PlayerEntity.hpp"


namespace entities {

	void PlayerEntity::render(render::render_helpers::RenderUtils&& renderer) const {
		renderer.draw(this->position, [this](auto&& r) {
			r.set_color(this->player_color);
			r.move_x(4);
			r.move_y(3);
			r.move_x(-12);
			r.move_y(-3);
		});

		renderer.text(this->position, [](auto&& r) {
			r.set_color(render::default_colors::BLACK);
			r.set_color_bg(render::default_colors::CYAN);
			r.put(L"hello this is a test");
		});
	}

	void PlayerEntity::tick(float delta) {
		BaseEntity::tick(delta);

		bool hit_x =
			(this->position.x >= 90 && this->velocity.x > 0) || (this->position.x <= 0 && this->velocity.x < 0);
		bool hit_y =
			(this->position.y >= 25 && this->velocity.y > 0) || (this->position.y <= 0 && this->velocity.y < 0);

		if (hit_x)
			this->velocity.x *= -1.15;
		if (hit_y)
			this->velocity.y *= -1.15;

		if (hit_x || hit_y) {
			this->player_color = render::Color {
				static_cast<uint8_t>(rand() % 255),
				static_cast<uint8_t>(rand() % 255),
				static_cast<uint8_t>(rand() % 255)
			};
		}
	}

} // entities