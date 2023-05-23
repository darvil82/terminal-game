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

		this->enabled_optimization = !utils::Terminal::is_a_tty();

		this->resize(width, height);

		output_stream << utils::Terminal::CURSOR_HIDE
			<< utils::Terminal::BUFFER_NEW;

		this->push_stream();
	}

	Renderer::~Renderer() {
		this->stop_render_loop();

		output_stream << utils::Terminal::CURSOR_SHOW
			<< utils::Terminal::CLEAR_ALL
			<< utils::Terminal::BUFFER_OLD;

		this->push_stream();
		std::setlocale(LC_ALL, prev_locale.c_str());

		this->free_buff();
	}

	bool Renderer::is_in_bounds(const utils::SPoint& pos) const {
		return pos.x < this->buffer_width && pos.x >= 0 && pos.y < this->buffer_height && pos.y >= 0;
	}

	void Renderer::set_current_fps(uint8_t fps) {
		this->current_fps = std::max<uint8_t>(std::min(fps, this->max_fps), 1);
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
							? utils::Terminal::cursor_move_x(x - last_pixel_position.x - 1)
							: utils::Terminal::cursor_set_pos({x, y})
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
			}
		}

		auto str = buff.str();
		size_t size = str.length();

		// if nothing changed, don't push anything
		if (size == 0) return 0;

		output_stream << str;
		this->push_stream();

		return size;
	}

	void Renderer::render(std::function<void(const RenderUtils&)> func) {
		// just some values that seem to work well
		constexpr const double CHARS_PRINTED_THRESHOLD = 9000.0;
		constexpr const double FRAME_RATE_FACTOR = 0.3;

		timestamp last_frame_time;

		while (this->is_rendering) {
			const timestamp current_frame_time = chrono::steady_clock::now();
			const double frame_time = chrono::duration_cast<chrono::duration<double>>(
				current_frame_time - last_frame_time
			).count();
			last_frame_time = current_frame_time;

			this->clear_buffer();
			func(this->get_render_utils());
			auto num_printed_chars = this->push_buffer(this->force_render_next_frame);

			// limit the framerate exponentially based on how many characters were printed
			if (this->enabled_optimization && num_printed_chars > 0)
				this->set_current_fps(
					this->max_fps * std::pow(FRAME_RATE_FACTOR, num_printed_chars / CHARS_PRINTED_THRESHOLD)
				);

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

	const RenderUtils Renderer::get_render_utils() {
		return RenderUtils(*this);
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

	void Renderer::start_render_loop(std::function<void(const RenderUtils&)> func) {
		this->is_rendering = true;
		this->render_thread = std::thread(&Renderer::render, this, func);
	}

	void Renderer::stop_render_loop() {
		if (!this->is_rendering) return;
		this->is_rendering = false;
		this->render_thread.join();
	}


	namespace render_helpers {

	}
} // render