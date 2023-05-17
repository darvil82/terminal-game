#include <stdexcept>
#include <sstream>
#include <iostream>
#include <cmath>

#include "Renderer.hpp"
#include "../utils/Terminal.hpp"
#include "Pixel.hpp"
#include "../utils/Typedefs.hpp"


namespace render {
	Renderer::Renderer(buff_size_t width, buff_size_t height) {
		this->prev_locale = std::setlocale(LC_ALL, nullptr);
		std::setlocale(LC_ALL, "en_US.utf8");

		this->enabled_optimization = !Terminal::is_a_tty();

		this->resize(width, height);

		output_stream << Terminal::CURSOR_HIDE
			<< Terminal::BUFFER_NEW;

		this->push_stream();
	}

	Renderer::~Renderer() {
		this->stop_render_loop();

		output_stream << Terminal::CURSOR_SHOW
			<< Terminal::CLEAR_ALL
			<< Terminal::BUFFER_OLD;

		this->push_stream();
		std::setlocale(LC_ALL, prev_locale.c_str());

		this->free_buff();
	}

	bool Renderer::is_in_bounds(const utils::SPoint& pos) const {
		return pos.x < this->buffer_width && pos.x >= 0 && pos.y < this->buffer_height && pos.y >= 0;
	}

	void Renderer::set_current_fps(uint8_t fps) {
		this->current_fps = std::max(std::min(fps, this->max_fps), static_cast<uint8_t>(1));
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
		this->force_render_next_frame = true;
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
				this->current_buffer[y][x] = this->background_pixel;
			}
		}
	}

	uint16_t Renderer::push_buffer(bool force_render) {
		std::wstringstream buff;
		const Pixel* last_pixel = nullptr;
		utils::Point<buff_size_t> last_pixel_position = {0, 0};
		uint16_t adjacent_streak = 0; // number streak of adjacent pixels placed
		uint16_t pixels_changed_count = 0; // number of pixels changed in this frame

		for (buff_size_t y = 0; y < this->buffer_height; y++) {
			for (buff_size_t x = 0; x < this->buffer_width; x++) {
				const Pixel& current_pixel = this->current_buffer[y][x];
				const Pixel& prev_frame_pixel = this->previous_buffer[y][x];

				const bool is_unchanged = current_pixel == prev_frame_pixel;


				// if pixel hasn't changed since last frame, skip it
				if (is_unchanged && !force_render) {
					adjacent_streak = 0;
					continue;
				}

				/*
				 * if pixel now needs to be placed at x0 (first pixel in line) and it is adjacent to the previous
				 * one, this means we are wrapping to next line, so we need to place cursor there.
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
						(last_pixel && last_pixel_position.y == y)
							? Terminal::cursor_move_x(x - last_pixel_position.x - 1)
							: Terminal::cursor_set_pos({x, y})
					);
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
				last_pixel_position = {x, y};
				adjacent_streak++;
				pixels_changed_count++;
			}
		}

		// if nothing changed, don't push anything
		if (pixels_changed_count == 0) return 0;

		output_stream << buff.str();
		this->push_stream();

		return pixels_changed_count;
	}

	void Renderer::render(std::function<void(const render_helpers::RenderUtils&)> func) {
		constexpr const double PIXEL_CHANGE_THRESHOLD = 500.0;
		constexpr const double FRAME_RATE_FACTOR = 0.4;
		timestamp last_frame_time;

		while (this->is_rendering) {
			const timestamp current_frame_time = chrono::steady_clock::now();
			const double frame_time = chrono::duration_cast<chrono::duration<double>>(
				current_frame_time - last_frame_time).count();
			last_frame_time = current_frame_time;

			this->clear_buffer();
			func(this->get_render_utils());
			auto changed_pixels = this->push_buffer(this->force_render_next_frame);

			// limit the framerate exponentially based on how many pixels changed
			if (this->enabled_optimization && changed_pixels > 0)
				this->set_current_fps(this->max_fps * pow(FRAME_RATE_FACTOR, changed_pixels / PIXEL_CHANGE_THRESHOLD));

			// cap framerate to current max fps. make sure we don't wait the first frame
			if (frame_time < 1.0f / this->current_fps) {
				std::this_thread::sleep_for(chrono::duration<float>(1.0f / this->current_fps - frame_time));
			}

			this->force_render_next_frame = false;
		}
	}

	void Renderer::push_stream() {
		std::wcout << output_stream.str() << std::flush;
		output_stream.str(L"");
	}

	void Renderer::set_background_pixel(const Pixel& pixel) {
		this->background_pixel = pixel;
	}

	const render_helpers::RenderUtils Renderer::get_render_utils() {
		return render_helpers::RenderUtils(*this);
	}

	void Renderer::set_max_fps(uint8_t fps) {
		this->max_fps = fps;
	}

	uint8_t Renderer::get_max_fps() const {
		return this->max_fps;
	}

	uint8_t Renderer::get_current_fps() const {
		return this->current_fps;
	}

	void Renderer::start_render_loop(std::function<void(const render_helpers::RenderUtils&)> func) {
		this->is_rendering = true;
		this->render_thread = std::thread(&Renderer::render, this, func);
	}

	void Renderer::stop_render_loop() {
		if (!this->is_rendering) return;
		this->is_rendering = false;
		this->render_thread.join();
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