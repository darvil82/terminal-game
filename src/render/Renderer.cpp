#include <stdexcept>
#include <sstream>
#include <iostream>

#include "Renderer.hpp"
#include "TerminalSequences.hpp"
#include "Pixel.hpp"


namespace render {
	Renderer::Renderer(buff_size_t width, buff_size_t height) {
		this->prev_locale = std::setlocale(LC_ALL, nullptr);
		std::setlocale(LC_ALL, "en_US.utf8");

		this->resize(width, height);

		output_stream << TerminalSequences::CURSOR_HIDE
			<< TerminalSequences::BUFFER_NEW;

		this->push_stream();
	}

	Renderer::~Renderer() {
		output_stream << TerminalSequences::CURSOR_SHOW
			<< TerminalSequences::BUFFER_OLD;
		this->push_stream();
		std::setlocale(LC_ALL, prev_locale.c_str());

		this->free_buff();
	}

	bool Renderer::is_in_bounds(const utils::SPoint& pos) const {
		return pos.x < this->buffer_width && pos.x >= 0 && pos.y < this->buffer_height && pos.y >= 0;
	}

	void Renderer::free_buff() {
		if (!this->current_buffer) return;

		for (buff_size_t y = 0; y < this->buffer_height; y++) {
			// free rows of matrices
			delete[] this->current_buffer[y];
		}

		// free matrices
		delete[] this->current_buffer;
		this->current_buffer = nullptr;

		// free previous
		if (!this->previous_buffer) return;

		for (buff_size_t y = 0; y < this->buffer_height; y++) {
			// free rows of matrices
			delete[] this->previous_buffer[y];
		}

		// free matrices
		delete[] this->previous_buffer;
		this->previous_buffer = nullptr;
	}

	void Renderer::resize(buff_size_t new_width, buff_size_t new_height) {
		// unchanged... nothing to do
		if (this->current_buffer && new_height == this->buffer_height && new_width == this->buffer_width) return;

		// free previous
		this->free_buff();

		// create new one
		this->current_buffer = new Pixel* [new_height];
		for (buff_size_t y = 0; y < new_height; y++) {
			this->current_buffer[y] = new Pixel[new_width]; // default pixels have default background and foreground colors
		}

		// also create previous
		this->previous_buffer = new Pixel* [new_height];
		for (buff_size_t y = 0; y < new_height; y++) {
			this->previous_buffer[y] = new Pixel[new_width];
		}

		this->buffer_width = new_width;
		this->buffer_height = new_height;
	}

	std::tuple<Renderer::buff_size_t, Renderer::buff_size_t> Renderer::get_size() const {
		return {this->buffer_width, this->buffer_height};
	}

	void Renderer::set_pixel(const Pixel& pixel, const utils::SPoint& position) {
		if (!this->is_in_bounds(position)) return;

		this->current_buffer[position.y][position.x] = pixel;
	}

	const Pixel& Renderer::get_pixel(const utils::SPoint& pos) const {
		if (!this->is_in_bounds(pos))
			throw std::out_of_range("Pixel out of bounds");
		return this->current_buffer[pos.y][pos.x];
	}

	void Renderer::clear_buffer() {
		for (buff_size_t y = 0; y < this->buffer_height; y++) {
			for (buff_size_t x = 0; x < this->buffer_width; x++) {
				this->previous_buffer[y][x] = this->current_buffer[y][x]; // copy current to previous
				this->current_buffer[y][x] = Pixel(
					L' ', default_colors::WHITE, this->background_color
				);
			}
		}
	}

	void Renderer::push_buffer(bool force_render) {
		std::wstringstream buff;
		const Pixel* last_pixel = nullptr;
		utils::Point<buff_size_t> last_pixel_position = { 0, 0 };
		uint16_t adjacent_streak = 0; // number streak of adjacent pixels placed
		uint16_t pixels_changed_count = 0; // number of pixels changed in this frame

		for (buff_size_t y = 0; y < this->buffer_height; y++) {
			for (buff_size_t x = 0; x < this->buffer_width; x++) {
				const Pixel& current_pixel = this->current_buffer[y][x];
				const Pixel& prev_frame_pixel = this->previous_buffer[y][x];

				const bool is_unchanged = current_pixel == prev_frame_pixel;

				/*
				 * not worth it to do anything in here if the pixel is the first one to be placed.
				 * position isn't important either because the cursor is placed at 0,0 at the beginning of the frame.
				 */
				if (last_pixel) {
					// if pixel hasn't changed since last frame, skip it
					if (is_unchanged && !force_render) {
						adjacent_streak = 0;
						continue;
					}

					/*
					 * if pixel now needs to be placed at x0 (first pixel in line).
					 * If this one is adjacent to previous one, this means we are wrapping to next line,
					 * so we need to place cursor there.
					 *
					 * We could just send the set_pos sequence, but that would use more bytes to write than just a
					 * single '\n' character.
					 */
					if (x == 0 && adjacent_streak != 0) {
						buff << '\n'; // wrap to next line
					}

					// if pixel is not adjacent to previous one, we need to place cursor at its position
					if (adjacent_streak == 0) {
						buff << (
							/*
							 * if last pixel was on the same line, we can just move cursor to the right.
							 * we can save a few bytes to write here by doing this. the move_x writes fewer characters
							 * than the set_pos, since it doesn't need to write the y coordinate.
							 */
							(last_pixel_position.y == y)
								? TerminalSequences::cursor_move_x(x - last_pixel_position.x - 1)
								: TerminalSequences::cursor_set_pos({x, y})
						);
					}
				}


				/* only print color sequences if they are different from previous pixel */

				if (!last_pixel || last_pixel->color_fg != current_pixel.color_fg) {
					buff << current_pixel.color_fg.get_sequence();
				}

				if (!last_pixel || last_pixel->color_bg != current_pixel.color_bg) {
					buff << current_pixel.color_bg.get_sequence(true);
				}

				buff << current_pixel.character; // print character

				last_pixel = &current_pixel;
				last_pixel_position = { x, y };
				adjacent_streak++;
				pixels_changed_count++;
			}
		}

		// if nothing changed, don't push anything
		if (pixels_changed_count == 0) return;

		// move cursor to home position (0, 0)
		output_stream << TerminalSequences::CURSOR_HOME;
		output_stream << buff.str();
		this->push_stream();
	}

	void Renderer::render() {
		this->push_buffer(this->force_render_next_frame);

		this->force_render_next_frame = false;
	}

	void Renderer::push_stream() {
		std::wcout << output_stream.str() << std::flush;
		output_stream.str(L"");
	}

	void Renderer::set_background_color(const Color& color) {
		this->background_color = color;
	}

	const render_helpers::RenderUtils Renderer::get_render_utils() {
		return render_helpers::RenderUtils(*this);
	}


	namespace render_helpers {

		void RenderUtils::draw(const utils::SPoint& start_pos, OpFunc<DrawOperation> draw_func) const {
			draw_func({this->renderer, start_pos});
		}

		void RenderUtils::text(const utils::SPoint& start_pos, OpFunc<TextOperation> draw_func) const {
			draw_func({this->renderer, start_pos});
		}

		const Renderer& RenderUtils::get_renderer() const {
			return this->renderer;
		}

		void RenderOperationBase::push_changes() {
			this->renderer.set_pixel({
				this->current_char,
				this->current_color,
				this->current_color_bg
			}, this->current_pos);
		}

		void RenderOperationBase::set_color(const Color& color) {
			this->current_color = color;
			this->push_changes();
		}

		void RenderOperationBase::set_color_bg(const Color& color) {
			this->current_color_bg = color;
			this->push_changes();
		}

		void RenderOperationBase::set_position_relative(const utils::Point<int16_t>& offset) {
			this->current_pos += offset;
			this->push_changes();
		}

		void RenderOperationBase::set_position(const utils::SPoint& pos) {
			this->current_pos = pos;
			this->push_changes();
		}

		void DrawOperation::move_x(int16_t offset) {
			if (offset == 0) return;

			if (offset > 0) {
				for (int16_t i = 0; i < offset; i++) {
					this->current_pos.x++;
					this->push_changes();
				}
				return;
			}

			for (int16_t i = 0; i > offset; i--) {
				this->current_pos.x--;
				this->push_changes();
			}
		}

		void DrawOperation::move_y(int16_t offset) {
			if (offset == 0) return;

			if (offset > 0) {
				for (int16_t i = 0; i < offset; i++) {
					this->current_pos.y++;
					this->push_changes();
				}
				return;
			}

			for (int16_t i = 0; i > offset; i--) {
				this->current_pos.y--;
				this->push_changes();
			}
		}

		void DrawOperation::set_char(wchar_t character) {
			this->current_char = character;
			this->push_changes();
		}

		void DrawOperation::rect(const utils::SPoint& size) {
			auto start_pos = this->current_pos;
			for (int16_t y = start_pos.y; y < size.y + start_pos.y; y++) {
				for (int16_t x = start_pos.x; x < size.x + start_pos.x; x++) {
					this->set_position({x, y});
				}
			}
		}

		void TextOperation::put(const std::wstring& content) {
			for (size_t i = 0; i < content.length(); i++) {
				this->current_char = content[i];
				this->push_changes();
				this->current_pos.x++;
			}
		}

		void TextOperation::put_line(const std::wstring& content) {
			this->put(content);
			this->current_pos.x -= content.length(); // move back to the start of the line
			this->current_pos.y++; // move down one line
		}
	}
} // render