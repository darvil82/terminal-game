#pragma once

#include <inttypes.h>
#include <string>
#include <sstream>
#include <optional>
#include <functional>
#include <tuple>
#include <thread>

#include "Characters.hpp"
#include "IRenderSequence.hpp"
#include "../utils/Point.hpp"
#include "Pixel.hpp"
#include "../utils/Concepts.hpp"

namespace utils {
	template<class T>
	struct Point;
}

namespace render {
	namespace render_helpers {
		class RenderUtils;
	}

	class Renderer {
		using buff_size_t = uint16_t;

		std::string prev_locale; // previous locale
		std::wstringstream output_stream;
		std::thread render_thread;
		buff_size_t buffer_width = 50, buffer_height = 50;
		Pixel** current_buffer = nullptr; // pixel matrix
		Pixel** previous_buffer = nullptr; // previous frame
		Pixel background_pixel = {
			default_characters::SPACE, default_colors::WHITE, default_colors::BLACK
		};
		bool force_render_next_frame = true; // usually just used for first frame
		bool is_rendering = false;
		uint8_t max_fps = 60, current_fps = 60;

		void free_buff();
		bool is_in_bounds(const utils::SPoint& pos) const;
		void push_stream();
		void clear_buffer();
		uint16_t push_buffer(bool force_render = false);
		void render(std::function<void(const render_helpers::RenderUtils&)> func);

	public:
		Renderer(buff_size_t width, buff_size_t height);
		~Renderer();

		std::tuple<buff_size_t, buff_size_t> get_size() const;
		void set_pixel(const Pixel& pixel, const utils::SPoint& position);
		const Pixel& get_pixel(const utils::SPoint& pos) const;
		void set_background_pixel(const Pixel& pixel);
		void set_max_fps(uint8_t fps);
		uint8_t get_max_fps() const;
		uint8_t get_current_fps() const;

		void resize(buff_size_t new_width, buff_size_t new_height);
		void start_render_loop(std::function<void(const render_helpers::RenderUtils&)> func);
		void stop_render_loop();

		const render_helpers::RenderUtils get_render_utils();
	};

	namespace render_helpers {

		class RenderUtils;


		class RenderOperationBase {
			friend RenderUtils;

			Renderer& renderer;
		protected:
			utils::SPoint current_pos;
			Color current_color = default_colors::WHITE;
			Color current_color_bg = default_colors::BLACK;
			wchar_t current_char = default_characters::blocks::FULL;

			void push_changes();

		public:
			RenderOperationBase(Renderer& r, const utils::SPoint& start_pos) : renderer {r}, current_pos {start_pos} { }

			void set_color(const Color& color);
			void set_color_bg(const Color& color);
			void set_position(const utils::SPoint& pos);
			void set_position_relative(const utils::Point<int16_t>& offset);
		};


		class DrawOperation : public RenderOperationBase {
			using RenderOperationBase::RenderOperationBase;
		public:
			void move_x(int16_t offset);
			void move_y(int16_t offset);
			void set_char(const wchar_t chr);
			void rect(const utils::SPoint& size);
		};


		class TextOperation : public RenderOperationBase {
			using RenderOperationBase::RenderOperationBase;
		public:
			void put(const std::wstring& content);
			void put_line(const std::wstring& content);
		};


		class RenderUtils {
			friend Renderer;
			Renderer& renderer;

			RenderUtils(Renderer& r) : renderer {r} { }

			template<Extends<RenderOperationBase> T>
			using OpFunc = std::function<void(T&&)>;

		public:
			const Renderer& get_renderer() const;

			void draw(const utils::SPoint& start_pos, OpFunc<DrawOperation> draw_func) const;
			void text(const utils::SPoint& start_pos, OpFunc<TextOperation> text_func) const;
		};
	}


} // render