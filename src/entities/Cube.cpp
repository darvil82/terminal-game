#include <math.h>
#include "Cube.hpp"
#include "../input/InputSystem.hpp"
#include "../utils/Math.hpp"

namespace entities {

	void Cube::render(const render::render_helpers::RenderUtils& render_utils) const {
		render_utils.draw(this->position, [this](auto&& r) {
			r.set_color(this->color);
//			r.rect({4, 2});
		});

//		render_utils.text(this->position, [this](auto&& r) {
//			r.put_line(L"Vel: " + std::to_wstring(this->velocity.x) + L" " + std::to_wstring(this->velocity.y));
//			r.put_line(L"Pos: " + std::to_wstring(this->position.x) + L" " + std::to_wstring(this->position.y));
//		});
	}

	void Cube::tick(float delta) {
		BaseEntity::tick(delta);

		bool hit_x =
			(this->position.x >= 120 && this->velocity.x > 0) || (this->position.x <= 0 && this->velocity.x < 0);
		bool hit_y =
			(this->position.y >= 30 && this->velocity.y > 0) || (this->position.y <= 0 && this->velocity.y < 0);

		if (hit_x) this->velocity.x *= -1;
		if (hit_y) this->velocity.y *= -1;

		// slowly decrease velocity
		this->velocity *= pow(0.8, delta);

		// apply gravity
		this->velocity.y += 30 * delta;

		bool is_on_ground = this->position.y >= 30;

		if (is_on_ground) {
			this->position.y = 30;
			this->velocity.x *= pow(0.005, delta);
		}

	}

	void Cube::jump() {
		this->velocity.x = rand() % 100 - 50;
		this->velocity.y = rand() % 100 - 50;
	}

} // entities