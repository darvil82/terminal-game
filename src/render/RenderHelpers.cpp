#include "RenderHelpers.hpp"

namespace render {
	namespace render_helpers {
		DrawRenderHelper::This&& DrawRenderHelper::start() {
			this->add_action([this] (Renderer& r) {
				this->is_drawing = true;
				this->push_pixel(r);
			});
			return static_cast<This&&>(*this);
		}

		DrawRenderHelper::This&& DrawRenderHelper::stop() {
			this->add_action([this] (Renderer&) {
				this->is_drawing = false;
			});
			return static_cast<This&&>(*this);
		}

		DrawRenderHelper::This&& DrawRenderHelper::move_x(int16_t dist) {
			if (dist == 0) return static_cast<This&&>(*this);

			this->add_action([=, this] (Renderer& r) {
				if (!this->is_drawing) {
					this->position.x += dist;
					return;
				}

				for (int16_t i = 1; i < std::abs(dist); i++) {
					this->position.x += math::sign(dist);
					this->push_pixel(r);
				}
			});

			return static_cast<This&&>(*this);
		}

		DrawRenderHelper::This&& DrawRenderHelper::move_y(int16_t dist) {
			if (dist == 0) return static_cast<This&&>(*this);

			this->add_action([=, this] (Renderer& r) {
				if (dist == 0) return;

				if (!this->is_drawing) {
					this->position.y += dist;
					return;
				}

				for (int16_t i = 1; i < std::abs(dist); i++) {
					this->position.y += math::sign(dist);
					this->push_pixel(r);
				}
			});

			return static_cast<This&&>(*this);
		}
	}
}