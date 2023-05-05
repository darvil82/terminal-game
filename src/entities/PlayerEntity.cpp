#include "PlayerEntity.hpp"
#include "../input/InputSystem.hpp"
#include "../utils/Math.hpp"

namespace entities {

	void PlayerEntity::render(const render::render_helpers::RenderUtils& renderer) const {
		renderer.draw(this->position, [this](auto&& r) {
			r.set_color(this->player_color);
			r.move_x(4);
			r.move_y(3);
			r.move_x(-12);
			r.move_y(-3);
		});

		renderer.text({0, 0}, [this](auto&& r) {
			r.put(L"Vel: " + std::to_wstring(this->velocity.x) + L" " + std::to_wstring(this->velocity.y));
			r.set_position({0, 1});
			r.put(L"Pos: " + std::to_wstring(this->position.x) + L" " + std::to_wstring(this->position.y));
		});

	}

	void PlayerEntity::tick(float delta) {
		BaseEntity::tick(delta);

		bool hit_x =
			(this->position.x >= 90 && this->velocity.x > 0) || (this->position.x <= 0 && this->velocity.x < 0);

		if (hit_x) this->velocity.x *= -1;

		// slowly decrease velocity
		this->velocity *= 0.997;

		// apply gravity
		this->velocity.y += 0.25;

		bool is_on_ground = this->position.y >= 24;

		if (is_on_ground) {
			if (INPUT_IS_PRESSED('a')) this->velocity.x = -50;
			if (INPUT_IS_PRESSED('d')) this->velocity.x = 50;

			this->position.y = 24;
			this->velocity.x *= 0.900; // friction
			this->velocity.y = 0;
			this->jumped = 0;
		}

		// allow jumping on air
		if (INPUT_IS_PRESSED(' ') && this->jumped < 2) {
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