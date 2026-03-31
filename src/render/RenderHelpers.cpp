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
			return this->schedule([=] (This& self) {
				self.character = chr;
			});
		}

		DrawRenderHelper::This&& DrawRenderHelper::start() {
			return this->schedule([] (This& self) {
				self.is_drawing = true;
				self.push_changes();
			});
		}

		DrawRenderHelper::This&& DrawRenderHelper::put() {
			return this->schedule([] (This& self) {
				self.push_changes();
			});
		}

		DrawRenderHelper::This&& DrawRenderHelper::stop() {
			return this->schedule([] (This& self) {
				self.is_drawing = false;
			});
		}

		void DrawRenderHelper::move_position(bool is_x, int16_t offset) {
			if (offset == 0) return;

			this->schedule([captured_offset = offset, is_x] (This& self) {
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

			return this->schedule([=] (This& self) {
				self.thickness = new_thickness;

				if (self.is_drawing)
					self.push_changes();
			});
		}


		TextRenderHelper::This&& TextRenderHelper::set_alignment(Alignment new_alignment) {
			return this->schedule([=] (This& self) {
				self.alignment = new_alignment;
			});
		}

		size_t TextRenderHelper::get_line_x_offset(const std::string& text) const {
			if (this->alignment == Alignment::RIGHT)
				return text.length();
			else if (this->alignment == Alignment::CENTER)
				return text.length() / 2;
			else return 0;
		}

		TextRenderHelper::This&& TextRenderHelper::put(const std::string& text) {
			return this->schedule([=] (This& self) {
				self.position.x -= self.get_line_x_offset(text);

				for (auto& chr : text) {
					self.character = chr;
					self.push_changes();
					self.position.x++;
				}
			});
		}

		TextRenderHelper::This&& TextRenderHelper::put_line(const std::string& text) {
			// then jump to next line
			return this->schedule([&text] (This& self) {
				self.put(text); // first put text
				self.position.x = self.initial_pos.x; // reset x position
				self.position.y++; // jump to next line
			});
		}
	}
}