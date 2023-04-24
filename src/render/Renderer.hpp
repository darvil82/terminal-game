#pragma once

#include <inttypes.h>
#include <string>
#include <sstream>
#include <optional>
#include <functional>

#include "Characters.hpp"
#include "IRenderSequence.hpp"
#include "../utils/Point.hpp"

namespace utils {
	template<class T>
	struct Point;
}

namespace render {
	using RPoint = utils::Point<uint16_t>;


	struct Color : public IRenderSequence {
		uint8_t r, g, b;

		constexpr Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) :
			r {r}, g {g}, b {b} {
		}

		bool operator==(const Color& other) const;

		std::wstring get_sequence(bool background) const;
		std::wstring get_sequence() const override;
	};

	namespace default_colors {
		constexpr Color BLACK {0, 0, 0};
		constexpr Color WHITE {255, 255, 255};
		constexpr Color RED {255, 0, 0};
		constexpr Color GREEN {0, 255, 0};
		constexpr Color BLUE {0, 0, 255};
		constexpr Color YELLOW {255, 255, 0};
		constexpr Color MAGENTA {255, 0, 255};
	}

	struct Pixel : public IRenderSequence {
		Color color_fg;
		Color color_bg;
		wchar_t character;

		Pixel(
			const wchar_t character,
			const Color& fg_color,
			const Color& bg_color
		) : color_fg {fg_color}, color_bg {bg_color}, character {character} { }

		std::wstring get_sequence() const override;
	};


	class Renderer {
		using buff_size_t = uint16_t;

		std::wstringstream output_stream;
		buff_size_t buffer_width = 50, buffer_height = 50;
		const Pixel*** buffer = nullptr; // pixel matrix
		bool buffer_changed = false;

		void free_buff();
		bool is_in_bounds(const render::RPoint& pos) const;
		void push_stream();

	public:
		Renderer(buff_size_t width, buff_size_t height);
		~Renderer();

		void resize(buff_size_t new_width, buff_size_t new_height);
		void set_pixel(const Pixel& pixel, const RPoint& position);
		const Pixel& get_pixel(const RPoint& pos) const;
		void clear_buffer();
		void push_buffer();
		void init();
		void end();
	};


	class RenderHelper {
		Renderer& renderer;

		class DrawHelper {
			friend RenderHelper;
			Renderer& renderer;

			RPoint current_pos;
			Color current_color;
			Color current_color_bg;
			wchar_t current_char;

			void push_changes();

			DrawHelper(Renderer& r, const RPoint& start_pos): renderer{r}, current_pos{start_pos} {}
		public:
			void set_color(const Color& color);
			void set_color_bg(const Color& color);
			void set_char(const wchar_t chr);
			void set_position(const RPoint& pos);
			void set_position_relative(const utils::Point<int16_t>& offset);
			void move_x(int16_t offset);
			void move_y(int16_t offset);
		};
	public:
		RenderHelper(Renderer& r): renderer{r} {}

		void draw(const RPoint& start_pos, std::function<void(DrawHelper&&)> draw_func);
		void text(const RPoint& position, const std::wstring& text);
	};

} // render