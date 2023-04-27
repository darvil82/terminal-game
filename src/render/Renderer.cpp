export module render.renderer;

import <string>;
import <sstream>;
import <iostream>;
import <functional>;

import utils.point;
import render.characters;
import render.sequences;
import render.pixel;
import utils.concepts;

namespace render {
	export using RPoint = utils::Point<uint16_t>;

	class Renderer {
		using buff_size_t = uint16_t;

		std::wstringstream output_stream;
		buff_size_t buffer_width = 50, buffer_height = 50;
		const Pixel*** buffer = nullptr; // pixel matrix
		bool buffer_changed = false;

		void free_buff() {
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

		bool is_in_bounds(const render::RPoint& pos) const {
			return pos.x < this->buffer_width && pos.x >= 0 && pos.y < this->buffer_height && pos.y >= 0;
		}

		void push_stream() {
			std::wcout << output_stream.str() << std::flush;
			output_stream.str(L"");
		}
		void add_reset_colors() {
			output_stream << default_colors::WHITE.get_sequence()
				<< default_colors::BLACK.get_sequence(true);
		}

	public:
		Renderer(buff_size_t width, buff_size_t height) {
			this->resize(width, height);
		}

		~Renderer() {
			this->free_buff();
		}

		void resize(buff_size_t new_width, buff_size_t new_height) {
			// unchanged... nothing to do
			if (this->buffer && new_height == this->buffer_height && new_width == this->buffer_width) return;

			// free previous
			this->free_buff();

			// create new one
			this->buffer = new const Pixel** [new_height];
			for (buff_size_t y = 0; y < new_height; y++) {
				this->buffer[y] = new const Pixel* [new_width] { };
			}

			this->buffer_width = new_width;
			this->buffer_height = new_height;
			this->buffer_changed = false;
		}
		void set_pixel(const Pixel& pixel, const RPoint& position) {
			if (!this->is_in_bounds(position)) return;
			this->buffer[position.y][position.x] = new const Pixel(pixel);
			this->buffer_changed = true;
		}
		const Pixel& get_pixel(const RPoint& pos) const {
			if (!this->is_in_bounds(pos))
				throw std::out_of_range("Pixel out of bounds");
			return *this->buffer[pos.y][pos.x];
		}
		void clear_buffer() {
			for (buff_size_t y = 0; y < this->buffer_height; y++) {
				for (buff_size_t x = 0; x < this->buffer_width; x++) {
					if (!this->buffer[y][x]) continue;

					delete this->buffer[y][x];
					this->buffer[y][x] = nullptr;
				}
			}
			this->buffer_changed = false;
		}
		void push_buffer() {
			if (!this->buffer_changed) return; // don't need to push if nothing changed
			const Pixel* prev_pixel = nullptr;

			output_stream << TerminalSequences::CURSOR_HOME;

			for (buff_size_t y = 0; y < this->buffer_height; y++) {
				for (buff_size_t x = 0; x < this->buffer_width; x++) {
					const Pixel* current_pixel = this->buffer[y][x];

					if (!current_pixel) {
						// make sure we reset the background to prevent a mess.
						// we don't need to if the bg color is already black!
						if (prev_pixel && prev_pixel->color_bg != default_colors::BLACK) {
							output_stream << default_colors::BLACK.get_sequence(true);
							prev_pixel = nullptr;
						}
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

			this->add_reset_colors();
			this->push_stream();
		}
		void init() {
			output_stream << TerminalSequences::CURSOR_HIDE
				<< TerminalSequences::BUFFER_NEW;

			// we always start_loop with a black bg and white fg
			this->add_reset_colors();
			this->push_stream();
		}
		void end() {
			output_stream << TerminalSequences::CURSOR_SHOW
				<< TerminalSequences::BUFFER_OLD;
			this->push_stream();
		}

		render_helpers::RenderUtils get_render_utils() {
			return render_helpers::RenderUtils(*this);
		}
	};

	namespace render_helpers {

		export class RenderUtils;


		class RenderOperationBase {
			friend RenderUtils;

			Renderer& renderer;
		protected:
			RPoint current_pos;
			Color current_color = default_colors::WHITE;
			Color current_color_bg = default_colors::BLACK;
			wchar_t current_char = default_characters::blocks::FULL;

			void push_changes() {
				this->renderer.set_pixel({
					this->current_char,
					this->current_color,
					this->current_color_bg
				}, this->current_pos);
			}

		public:
			RenderOperationBase(Renderer& r, const RPoint& start_pos) : renderer {r}, current_pos {start_pos} { }

			void set_color(const Color& color) {
				this->current_color = color;
				this->push_changes();
			}

			void set_color_bg(const Color& color) {
				this->current_color_bg = color;
				this->push_changes();
			}
			void set_position(const RPoint& pos) {
				this->current_pos = pos;
				this->push_changes();
			}
			void set_position_relative(const utils::Point<int16_t>& offset) {
				this->current_pos += offset;
				this->push_changes();
			}
		};


		class DrawOperation : public RenderOperationBase {
			using RenderOperationBase::RenderOperationBase;
		public:
			void move_x(int16_t offset) {
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
			void move_y(int16_t offset) {
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
			void set_char(const wchar_t chr) {
				this->current_char = chr;
				this->push_changes();
			}
		};


		class TextOperation : public RenderOperationBase {
			using RenderOperationBase::RenderOperationBase;
		public:
			void put(const std::wstring& content) {
				for (size_t i = 0; i < content.length(); i++) {
					this->current_char = content[i];
					this->push_changes();
					this->current_pos.x++;
				}
			}
		};


		export class RenderUtils {
			friend Renderer;
			Renderer& renderer;

			RenderUtils(Renderer& r) : renderer {r} { }

			template<Extends<RenderOperationBase> T>
			using OpFunc = std::function<void(T&&)>;

		public:

			void draw(const RPoint& start_pos, OpFunc<DrawOperation> draw_func) const {
				draw_func({this->renderer, start_pos});
			}
			void text(const RPoint& start_pos, OpFunc<TextOperation> text_func) const {
				text_func({this->renderer, start_pos});
			}
		};
	}


} // render