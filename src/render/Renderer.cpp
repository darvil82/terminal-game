#include <stdexcept>
#include <sstream>
#include <iostream>

#include "Renderer.hpp"


namespace render {
	std::wstring Color::get_sequence() const {
		std::wstringstream buff;
		buff << ESCAPE_SEQUENCE_START
			<< (this->background ? "48" : "38") << ";2;"
			<< this->r << ';'
			<< this->g << ';'
			<< this->b << 'm';
		return buff.str();
	}

	bool Color::operator==(const render::Color& other) const {
		return this->r == other.r && this->g == other.g && this->b == other.b;
	}


	std::wstring RPoint::get_sequence() const {
		std::wstringstream buff;
		buff << ESCAPE_SEQUENCE_START
			<< this->y << ';'
			<< this->x << 'f';
		return buff.str();
	}

	std::wstring Pixel::get_sequence() const {
		std::wstringstream buff;
		buff << this->pos.get_sequence()
			<< this->color.get_sequence()
			<< this->character;
		return buff.str();
	}


	Renderer::Renderer(uint16_t width, uint16_t height) {
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

		for (uint16_t y = 0; y < this->buffer_height; y++) {
			for (uint16_t x = 0; x < this->buffer_width; x++) {
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

	void Renderer::resize(uint16_t new_width, uint16_t new_height) {
		// unchanged... nothing to do
		if (this->buffer && new_height == this->buffer_height && new_width == this->buffer_width) return;

		// free previous
		this->free_buff();

		// create new one
		this->buffer = new const Pixel** [new_height];
		for (uint16_t y = 0; y < new_height; y++) {
			this->buffer[y] = new const Pixel* [new_width];

			for (uint16_t x = 0; x < new_width; x++) {
				this->buffer[y][x] = nullptr;
			}
		}

		this->buffer_width = new_width;
		this->buffer_height = new_height;
	}

	void Renderer::set_pixel(const Pixel& pixel) {
		if (!this->is_in_bounds(pixel.pos)) return;
		this->buffer[pixel.pos.y][pixel.pos.x] = new const Pixel(pixel);
	}

	const Pixel& Renderer::get_pixel(const RPoint pos) const {
		if (!this->is_in_bounds(pos))
			throw std::out_of_range("Pixel out of bounds");
		return *this->buffer[pos.y][pos.x];
	}

	void Renderer::clear_all() {
		for (uint16_t y = 0; y < this->buffer_height; y++) {
			for (uint16_t x = 0; x < this->buffer_width; x++) {
				if (!this->buffer[y][x]) continue;

				delete this->buffer[y][x];
				this->buffer[y][x] = nullptr;
			}
		}
	}

	void Renderer::push_buffer() {
		const Pixel* prev_pixel = nullptr;
		std::wstringstream buff;

		for (uint16_t y = 0; y < this->buffer_height; y++) {
			for (uint16_t x = 0; x < this->buffer_width; x++) {
				auto* current_pixel = this->buffer[y][x];
				if (!current_pixel) {
					buff << ' ';
					continue;
				}

				// if the previous char color is the same, we don't need to add the color sequence again
				if (!prev_pixel || current_pixel->color != prev_pixel->color) {
					buff << current_pixel->color.get_sequence();
				}

				buff << current_pixel->character;

				prev_pixel = current_pixel;
			}
			buff << '\n';
		}

		std::wcout << buff.str() << std::flush;
	}
} // render