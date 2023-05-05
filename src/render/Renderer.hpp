#pragma once

#include <inttypes.h>
#include <string>
#include <sstream>
#include <optional>
#include <functional>

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
	using RPoint = utils::Point<uint16_t>;

	namespace render_helpers {
		class RenderUtils;
	}

	class Renderer {
		using buff_size_t = uint16_t;

		std::string prev_locale;
		std::wstringstream output_stream;
		buff_size_t buffer_width = 50, buffer_height = 50;
		const Pixel*** buffer = nullptr; // pixel matrix
		bool buffer_changed = false;

		void free_buff();
		bool is_in_bounds(const render::RPoint& pos) const;
		void push_stream();
		void add_reset_colors();

	public:
		Renderer(buff_size_t width, buff_size_t height);
		~Renderer();

		void resize(buff_size_t new_width, buff_size_t new_height);
		void set_pixel(const Pixel& pixel, const RPoint& position);
		const Pixel& get_pixel(const RPoint& pos) const;
		void clear_buffer();
		void push_buffer();

		const render_helpers::RenderUtils get_render_utils();
	};

	namespace render_helpers {

		class RenderUtils;


		class RenderOperationBase {
			friend RenderUtils;

			Renderer& renderer;
		protected:
			RPoint current_pos;
			Color current_color = default_colors::WHITE;
			Color current_color_bg = default_colors::BLACK;
			wchar_t current_char = default_characters::blocks::FULL;

			void push_changes();

		public:
			RenderOperationBase(Renderer& r, const RPoint& start_pos) : renderer {r}, current_pos {start_pos} { }

			void set_color(const Color& color);
			void set_color_bg(const Color& color);
			void set_position(const RPoint& pos);
			void set_position_relative(const utils::Point<int16_t>& offset);
		};


		class DrawOperation : public RenderOperationBase {
			using RenderOperationBase::RenderOperationBase;
		public:
			void move_x(int16_t offset);
			void move_y(int16_t offset);
			void set_char(const wchar_t chr);
		};


		class TextOperation : public RenderOperationBase {
			using RenderOperationBase::RenderOperationBase;
		public:
			void put(const std::wstring& content);
		};


		class RenderUtils {
			friend Renderer;
			Renderer& renderer;

			RenderUtils(Renderer& r) : renderer {r} { }

			template<Extends<RenderOperationBase> T>
			using OpFunc = std::function<void(T&&)>;

		public:

			void draw(const RPoint& start_pos, OpFunc<DrawOperation> draw_func) const;
			void text(const RPoint& start_pos, OpFunc<TextOperation> text_func) const;
		};
	}


} // render