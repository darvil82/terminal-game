#include <math.h>
#include "PlayerEntity.hpp"
#include "../input/InputSystem.hpp"
#include "../utils/Math.hpp"

namespace entities {

	void PlayerEntity::render(const render::render_helpers::RenderUtils& render_utils) const {
		render_utils.draw(this->position, [this](auto&& r) {
			r.set_color(this->player_color);
			r.move_x(4);
			r.move_y(3);
			r.move_x(-5);
			r.move_y(-3);
		});

		render_utils.text({0, 0}, [this](auto&& r) {
			r.put(L"Vel: " + std::to_wstring(this->velocity.x) + L" " + std::to_wstring(this->velocity.y));
			r.set_position({0, 1});
			r.put(L"Pos: " + std::to_wstring(this->position.x) + L" " + std::to_wstring(this->position.y));
			r.set_position({0, 2});
			r.put(L"Jumps: " + std::to_wstring(this->jumped));
		});
	}

	void PlayerEntity::tick(float delta) {
		BaseEntity::tick(delta);

		bool hit_x =
			(this->position.x >= 90 && this->velocity.x > 0) || (this->position.x <= 0 && this->velocity.x < 0);

		if (hit_x) this->velocity.x *= -1;

		// slowly decrease velocity
		this->velocity *= pow(0.9, delta);

		// apply gravity
		this->velocity.y += 30 * delta;

		bool is_on_ground = this->position.y >= 24;

		if (is_on_ground) {
			if (INPUT_IS_PRESSED(LEFT)) this->velocity.x = -50;
			if (INPUT_IS_PRESSED(RIGHT)) this->velocity.x = 50;

			this->position.y = 24;
			this->velocity.x *= pow(0.005, delta);
			this->velocity.y = 0;
			this->jumped = 0;
		}

		// allow jumping on air
		if (INPUT_IS_PRESSED(SPACE) && this->jumped < 2) {
			this->velocity.y = -20; // jump
			this->jumped++;

			this->player_color = render::Color {
				static_cast<uint8_t>(rand() % 255),
				static_cast<uint8_t>(rand() % 255),
				static_cast<uint8_t>(rand() % 255)
			};
		}

	}

} // entities