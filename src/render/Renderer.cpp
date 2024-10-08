#include <stdexcept>
#include <sstream>
#include <iostream>
#include <cmath>

#include "Renderer.hpp"
#include "../utils/Terminal.hpp"
#include "Pixel.hpp"
#include "../utils/Typedefs.hpp"


namespace render {
	Renderer::Renderer(buff_size_t width, buff_size_t height, bool alternate_buffer) {
		this->use_alternate_buffer = alternate_buffer;

		this->resize(width, height);

		output_stream << utils::Terminal::prepare(alternate_buffer);

		this->push_stream();
	}

	Renderer::~Renderer() {
		this->stop_render_loop();

		output_stream << utils::Terminal::restore(this->use_alternate_buffer);

		this->push_stream();
		this->free_buff();
	}

	bool Renderer::is_in_bounds(const utils::SPoint& pos) const {
		return pos.x < this->buffer_width && pos.x >= 0 && pos.y < this->buffer_height && pos.y >= 0;
	}

	void Renderer::set_current_fps(uint8_t fps) {
		this->current_fps = std::max<uint8_t>(std::min(fps, this->max_fps), 1);
	}

	void Renderer::free_buff() {
		if (!this->global_buffer) return;

		delete[] this->global_buffer;
		this->global_buffer = nullptr;
	}

	void Renderer::resize(buff_size_t new_width, buff_size_t new_height) {
		// unchanged... nothing to do
		if (this->global_buffer && new_height == this->buffer_height && new_width == this->buffer_width) return;

		this->free_buff();

		const auto matrix_size = new_height*new_width;

		this->global_buffer = new Pixel[matrix_size*2];
		this->current_buffer = this->global_buffer;
		this->aux_buffer = this->global_buffer + matrix_size;

		this->buffer_width = new_width;
		this->buffer_height = new_height;
		this->force_render_next_frame = true;
	}

	Pixel& Renderer::get_pixel_from_buffer(const utils::SPoint& pos, Pixel* buff) {
		return buff[this->buffer_width*pos.y + pos.x];
	}

	const Pixel& Renderer::get_pixel_from_buffer(const utils::SPoint& pos, Pixel* buff) const {
		return buff[this->buffer_height*pos.y + pos.x];
	}

	std::tuple<Renderer::buff_size_t, Renderer::buff_size_t> Renderer::get_size() const {
		return {this->buffer_width, this->buffer_height};
	}

	void Renderer::set_pixel(const Pixel& pixel, const utils::SPoint& pos) {
		if (!this->is_in_bounds(pos)) return;

		this->get_pixel_from_buffer(pos, this->current_buffer) = pixel;
	}

	const Pixel& Renderer::get_pixel(const utils::SPoint& pos) const {
		if (!this->is_in_bounds(pos))
			throw std::out_of_range("Pixel out of bounds");
		return this->get_pixel_from_buffer(pos, this->current_buffer);
	}

	void Renderer::clear_buffer() {
		// swap buffers. move current to aux basically
		const auto aux_tmp = this->aux_buffer;
		this->aux_buffer = this->current_buffer;
		this->current_buffer = aux_tmp;

		// clear new
		for (buff_size_t y = 0; y < this->buffer_height; y++) {
			for (buff_size_t x = 0; x < this->buffer_width; x++) {
				this->get_pixel_from_buffer({x, y}, this->current_buffer) = this->background_pixel;
			}
		}
	}

	uint16_t Renderer::push_buffer(bool force_render) {
		std::stringstream buff;
		uint16_t adjacent_streak = 0; // number streak of adjacent pixels placed
		const Pixel* last_pixel_ptr = force_render ? nullptr : &this->last_pixel;

		for (buff_size_t y = 0; y < this->buffer_height; y++) {
			for (buff_size_t x = 0; x < this->buffer_width; x++) {
				const Pixel& current_pixel = this->get_pixel_from_buffer({x, y}, this->current_buffer);
				const Pixel& prev_frame_pixel = this->get_pixel_from_buffer({x, y}, this->aux_buffer);

				// if pixel hasn't changed since last frame, skip it
				if (!force_render && current_pixel == prev_frame_pixel) {
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
						(last_pixel_ptr && this->last_pixel_position.y == y)
							? utils::Terminal::cursor_move_x(x - this->last_pixel_position.x - 1)
							: utils::Terminal::cursor_set_pos({x, y})
					);
				}


				/* only print color sequences if they are different from previous pixel */

				if (!last_pixel_ptr || last_pixel_ptr->color_fg != current_pixel.color_fg) {
					buff << current_pixel.color_fg.get_sequence();
				}

				if (!last_pixel_ptr || last_pixel_ptr->color_bg != current_pixel.color_bg) {
					buff << current_pixel.color_bg.get_sequence(true);
				}

				buff << current_pixel.character.to_string(); // print character

				last_pixel_ptr = &current_pixel;
				this->last_pixel_position = {x, y};
				adjacent_streak++;
			}
		}

		auto str = buff.str();
		size_t size = str.length();

		// save a copy since when clearing the buffer the pointer will point to a new pixel
		if (last_pixel_ptr)
			this->last_pixel = *last_pixel_ptr;

		// if nothing changed, don't push_changes anything
		if (size == 0) return 0;

		output_stream << str;
		this->push_stream();

		return size;
	}

	void Renderer::render(std::function<void(Renderer&)> func) {
		// just some values that seem to work well
		const double CHARS_PRINTED_THRESHOLD = 9000.0;
		const double FRAME_RATE_FACTOR = 0.3;

		timestamp last_frame_time;

		while (this->is_rendering) {
			const timestamp current_frame_time = chrono::steady_clock::now();
			const double frame_time = chrono::duration_cast<chrono::duration<double>>(
				current_frame_time - last_frame_time
			).count();
			last_frame_time = current_frame_time;

			this->clear_buffer();
			func(*this);
			this->changed_pixels = this->push_buffer(this->force_render_next_frame);

			// limit the framerate exponentially based on how many characters were printed
			if (this->enabled_optimization && this->changed_pixels > 0)
				this->set_current_fps(
					this->max_fps * std::pow(FRAME_RATE_FACTOR, this->changed_pixels / CHARS_PRINTED_THRESHOLD)
				);

			// cap framerate to current max fps. make sure we don't wait the first frame
			if (frame_time < 1.0f / this->current_fps) {
				std::this_thread::sleep_for(chrono::duration<float>(1.0f / this->current_fps - frame_time));
			}

			this->force_render_next_frame = false;
		}
	}

	void Renderer::push_stream() {
		std::cout << output_stream.str() << std::flush;
		output_stream.str("");
	}

	void Renderer::set_background_pixel(const Pixel& pixel) {
		this->background_pixel = pixel;
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

	uint16_t Renderer::get_changed_pixels() const {
		return this->changed_pixels;
	}

	void Renderer::start_render_loop(std::function<void(Renderer&)> func) {
		this->is_rendering = true;
		this->render_thread = std::thread(&Renderer::render, this, func);
	}

	void Renderer::stop_render_loop() {
		if (!this->is_rendering) return;
		this->is_rendering = false;
		this->render_thread.join();
	}

	void Renderer::insert(render_helpers::IRenderHelper&& helper) {
		helper(*this);
	}

	Renderer& Renderer::operator<<(render_helpers::IRenderHelper&& helper) {
		this->insert(std::move(helper));
		return *this;
	}


	namespace render_helpers {

	}
} // render