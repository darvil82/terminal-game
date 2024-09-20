#include <iterator>
#include "RenderHelpers.hpp"

namespace render {
	namespace render_helpers {
		void DrawRenderHelper::push_changes() {
			if (this->thickness == 1) {
				BaseRenderHelper::push_changes();
				return;
			}

			uint16_t half_thickness = this->thickness / 2;

			for (int16_t y = this->position.y - half_thickness; y <= this->position.y + half_thickness; y++) {
				for (int16_t x = this->position.x - this->thickness; x <= this->position.x + this->thickness; x++) {
					this->set_pixel({x, y});
				}
			}
		}

		DrawRenderHelper::This&& DrawRenderHelper::set_character(UTF8Char chr) {
			this->add_action([=] (This& self) {
				self.character = chr;
			});
			return static_cast<This&&>(*this);
		}

		DrawRenderHelper::This&& DrawRenderHelper::start() {
			this->add_action([] (This& self) {
				self.is_drawing = true;
			});
			return this->put();
		}

		DrawRenderHelper::This&& DrawRenderHelper::put() {
			this->add_action([] (This& self) {
				self.push_changes();
			});
			return static_cast<This&&>(*this);
		}

		DrawRenderHelper::This&& DrawRenderHelper::stop() {
			this->add_action([] (This& self) {
				self.is_drawing = false;
			});
			return static_cast<This&&>(*this);
		}

		void DrawRenderHelper::move_position(bool is_x, int16_t offset) {
			if (offset == 0) return;

			this->add_action([captured_offset = offset, is_x] (This& self) {
				utils::SPoint::AxisType& axis = is_x ? self.position.x : self.position.y;
				int16_t offset = captured_offset * self.thickness;

				if (!self.is_drawing) {
					axis = offset;
					return;
				}

				for (int16_t i = 0; i < std::abs(offset); i++) {
					axis += math::sign(offset);
					self.push_changes();
				}
			});
		}


		DrawRenderHelper::This&& DrawRenderHelper::move_x(int16_t dist) {
			this->move_position(true, dist);
			return static_cast<This&&>(*this);
		}

		DrawRenderHelper::This&& DrawRenderHelper::move_y(int16_t dist) {
			this->move_position(false, dist);
			return static_cast<This&&>(*this);
		}

		DrawRenderHelper::This&& DrawRenderHelper::set_thickness(uint8_t new_thickness) {
			if (new_thickness < 1)
				throw std::invalid_argument("thickness cannot be less than 1");

			this->add_action([=] (This& self) {
				self.thickness = new_thickness;
			});

			return static_cast<This&&>(*this);
		}


		TextRenderHelper::This&& TextRenderHelper::set_alignment(Alignment new_alignment) {
			this->add_action([=] (This& self) {
				self.alignment = new_alignment;
			});
			return static_cast<This&&>(*this);
		}

		size_t TextRenderHelper::get_line_x_offset(const std::string& text) const {
			if (this->alignment == Alignment::RIGHT)
				return text.length();
			else if (this->alignment == Alignment::CENTER)
				return text.length() / 2;
			else return 0;
		}

		TextRenderHelper::This&& TextRenderHelper::put(const std::string& text) {
			this->add_action([=] (This& self) {
				self.position.x -= self.get_line_x_offset(text);

				for (auto& chr : text) {
					self.character = chr;
					self.push_changes();
					self.position.x++;
				}
			});

			return static_cast<This&&>(*this);
		}

		TextRenderHelper::This&& TextRenderHelper::put_line(const std::string& text) {
			this->put(text); // first append action to push_changes text

			// then jump to next line
			this->add_action([] (This& self) {
				self.position.x = self.initial_pos.x; // reset x position
				self.position.y++; // jump to next line
			});

			return static_cast<This&&>(*this);
		}
	}
}