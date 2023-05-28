#include "RenderHelpers.hpp"

namespace render {
	namespace render_helpers {
		void DrawRenderHelper::push_changes() {
			if (this->thickness == 1) {
				BaseRenderHelper::push_changes();
				return;
			}

			auto half_thickness = this->thickness / 2;

			for (int8_t y = this->position.y - half_thickness; y <= this->position.y + half_thickness; y++) {
				for (int8_t x = this->position.x - this->thickness; x <= this->position.x + this->thickness; x++) {
					this->set_pixel({x, y});
				}
			}
		}

		DrawRenderHelper::This&& DrawRenderHelper::set_character(wchar_t chr) {
			this->add_action([=, this]() {
				this->character = chr;
			});
			return static_cast<This&&>(*this);
		}

		DrawRenderHelper::This&& DrawRenderHelper::start() {
			this->add_action([this]() {
				this->is_drawing = true;
				this->push_changes();
			});
			return static_cast<This&&>(*this);
		}

		DrawRenderHelper::This&& DrawRenderHelper::stop() {
			this->add_action([this]() {
				this->is_drawing = false;
			});
			return static_cast<This&&>(*this);
		}

		DrawRenderHelper::This&& DrawRenderHelper::move_x(int16_t dist) {
			if (dist == 0) return static_cast<This&&>(*this);

			this->add_action([=, this]() {
				if (!this->is_drawing) {
					this->position.x += dist;
					return;
				}

				for (int16_t i = 1; i < std::abs(dist); i++) {
					this->position.x += math::sign(dist);
					this->push_changes();
				}
			});

			return static_cast<This&&>(*this);
		}

		DrawRenderHelper::This&& DrawRenderHelper::move_y(int16_t dist) {
			if (dist == 0) return static_cast<This&&>(*this);

			this->add_action([=, this]() {
				if (dist == 0) return;

				if (!this->is_drawing) {
					this->position.y += dist;
					return;
				}

				for (int16_t i = 1; i < std::abs(dist); i++) {
					this->position.y += math::sign(dist);
					this->push_changes();
				}
			});

			return static_cast<This&&>(*this);
		}

		DrawRenderHelper::This&& DrawRenderHelper::set_thickness(uint8_t t) {
			if (t < 1)
				throw std::invalid_argument("thickness cannot be less than 1");

			this->add_action([=, this]() {
				this->thickness = t;
			});

			return static_cast<This&&>(*this);
		}


		TextRenderHelper::This&& TextRenderHelper::put(const std::wstring& text) {
			this->add_action([&, this]() {
				for (auto& chr : text) {
					this->character = chr;
					this->push_changes();
					this->position.x++;
				}
			});

			return static_cast<This&&>(*this);
		}

		TextRenderHelper::This&& TextRenderHelper::put_line(const std::wstring& text) {
			this->put(text); // first append action to push_changes text

			// then jump to next line
			this->add_action([&, this]() {
				this->position.x -= text.length();
				this->position.y++;
			});

			return static_cast<This&&>(*this);
		}
	}
}