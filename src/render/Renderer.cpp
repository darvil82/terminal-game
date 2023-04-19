#include <stdexcept>
#include <sstream>
#include <iostream>

#include "Renderer.hpp"
#include "TerminalSequences.hpp"


namespace render {
	std::wstring Color::get_sequence(bool background) const {
		return TerminalSequences::set_color(*this, background);
	}

	std::wstring Color::get_sequence() const {
		return this->get_sequence(false);
	}

	bool Color::operator==(const render::Color& other) const {
		return this->r == other.r && this->g == other.g && this->b == other.b;
	}

	std::wstring Pixel::get_sequence() const {
		std::wstringstream buff;
		buff << this->color_fg.get_sequence()
			 << this->color_bg.get_sequence(true)
			 << this->character;
		return buff.str();
	}


	Renderer::Renderer(buff_size_t width, buff_size_t height) {
		this->resize(width, height);
	}

	Renderer::~Renderer() {
		this->free_buff();
	}

	bool Renderer::is_in_bounds(const render::RPoint& pos) const {
		return pos.x < this->buffer_width && pos.x >= 0 && pos.y < this->buffer_height && pos.y >= 0;
	}

	void Renderer::free_buff() {
		if (!this->buffer) return;

		for (buff_size_t y = 0; y < this->buffer_height; y++) {
			for (buff_size_t x = 0; x < this->buffer_width; x++) {
				// free each pixel
				delete this->buffer[y][x];
			}
			// free row
			delete[] this->buffer[y];
		}

		// free buffer
		delete[] this->buffer;
		this->buffer = nullptr;
	}

	void Renderer::resize(buff_size_t new_width, buff_size_t new_height) {
		// unchanged... nothing to do
		if (this->buffer && new_height == this->buffer_height && new_width == this->buffer_width) return;

		// free previous
		this->free_buff();

		// create new one
		this->buffer = new const Pixel** [new_height];
		for (buff_size_t y = 0; y < new_height; y++) {
			this->buffer[y] = new const Pixel* [new_width]{ };
		}

		this->buffer_width = new_width;
		this->buffer_height = new_height;
		this->buffer_changed = false;
	}

	void Renderer::set_pixel(const Pixel& pixel, const RPoint& position) {
		if (!this->is_in_bounds(position)) return;
		this->buffer[position.y][position.x] = new const Pixel(pixel);
		this->buffer_changed = true;
	}

	const Pixel& Renderer::get_pixel(const RPoint& pos) const {
		if (!this->is_in_bounds(pos))
			throw std::out_of_range("Pixel out of bounds");
		return *this->buffer[pos.y][pos.x];
	}

	void Renderer::clear_buffer() {
		for (buff_size_t y = 0; y < this->buffer_height; y++) {
			for (buff_size_t x = 0; x < this->buffer_width; x++) {
				if (!this->buffer[y][x]) continue;

				delete this->buffer[y][x];
				this->buffer[y][x] = nullptr;
			}
		}
		this->buffer_changed = false;
	}

	void Renderer::push_buffer() {
		if (!this->buffer_changed) return; // don't need to push if nothing changed
		const Pixel* prev_pixel = nullptr;

		output_stream << TerminalSequences::CURSOR_HOME;

		for (buff_size_t y = 0; y < this->buffer_height; y++) {
			for (buff_size_t x = 0; x < this->buffer_width; x++) {
				const Pixel* current_pixel = this->buffer[y][x];

				if (!current_pixel) {
					output_stream << ' ';
					continue;
				}

				// if the previous char colors are the same, we don't need to add the sequence again, just the char
				if (!prev_pixel || current_pixel->color_fg != prev_pixel->color_fg)
					output_stream << current_pixel->color_fg.get_sequence();

				if (!prev_pixel || current_pixel->color_bg != prev_pixel->color_bg)
					output_stream << current_pixel->color_bg.get_sequence(true);

				output_stream << current_pixel->character;
				prev_pixel = current_pixel;
			}
			output_stream << '\n';
		}

		this->push_stream();
	}

	void Renderer::push_stream() {
		std::wcout << output_stream.str() << std::flush;
		output_stream.str(L"");
	}

	void Renderer::init() {
		output_stream << TerminalSequences::CURSOR_HIDE
			<< TerminalSequences::BUFFER_NEW
			// we always start_loop with a black bg and white fg
			<< default_colors::WHITE.get_sequence()
			<< default_colors::BLACK.get_sequence(true);

		this->push_stream();
	}

	void Renderer::end() {
		output_stream << TerminalSequences::CURSOR_SHOW
			<< TerminalSequences::BUFFER_OLD;
		this->push_stream();
	}
} // render