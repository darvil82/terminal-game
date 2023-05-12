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
		if (!this->buffer) return;

		for (buff_size_t y = 0; y < this->buffer_height; y++) {
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
		this->buffer = new Pixel*[new_height];
		for (buff_size_t y = 0; y < new_height; y++) {
			this->buffer[y] = new Pixel[new_width]; // default pixels have default background and foreground colors
		}

		this->buffer_width = new_width;
		this->buffer_height = new_height;
		this->buffer_changed = false;
	}

	std::tuple<Renderer::buff_size_t, Renderer::buff_size_t> Renderer::get_size() const {
		return { this->buffer_width, this->buffer_height };
	}

	void Renderer::set_pixel(const Pixel& pixel, const utils::SPoint& position) {
		if (!this->is_in_bounds(position)) return;

		this->buffer[position.y][position.x] = pixel;
		this->buffer_changed = true;
	}

	const Pixel& Renderer::get_pixel(const utils::SPoint& pos) const {
		if (!this->is_in_bounds(pos))
			throw std::out_of_range("Pixel out of bounds");
		return this->buffer[pos.y][pos.x];
	}

	void Renderer::clear_buffer() {
		for (buff_size_t y = 0; y < this->buffer_height; y++) {
			for (buff_size_t x = 0; x < this->buffer_width; x++) {
				this->buffer[y][x] = Pixel(L' ', default_colors::WHITE, default_colors::BLACK);
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
				const Pixel& current_pixel = this->buffer[y][x];

				// if the previous char colors are the same, we don't need to add the sequence again, just the char
				if (!prev_pixel || current_pixel.color_fg != prev_pixel->color_fg)
					output_stream << current_pixel.color_fg.get_sequence();

				if (!prev_pixel || current_pixel.color_bg != prev_pixel->color_bg)
					output_stream << current_pixel.color_bg.get_sequence(true);

				output_stream << current_pixel.character;
				prev_pixel = &current_pixel;
			}
			output_stream << '\n';
		}

		this->push_stream();
	}

	void Renderer::push_stream() {
		std::wcout << output_stream.str() << std::flush;
		output_stream.str(L"");
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
			for (int16_t y = start_pos.y; y < size.y + start_pos.y ; y++) {
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