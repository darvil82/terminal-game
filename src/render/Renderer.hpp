#pragma once

#include <inttypes.h>
#include <string>
#include <sstream>
#include <optional>

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
		uint8_t r = 0, g = 0, b = 0;

		constexpr Color(uint8_t r, uint8_t g, uint8_t b) :
			r{r}, g{g}, b{b} {
		}

		bool operator==(const Color& other) const;

		std::wstring get_sequence(bool background) const;
		std::wstring get_sequence() const override;
	};

	namespace default_colors {
		constexpr Color BLACK{0, 0, 0};
		constexpr Color WHITE{255, 255, 255};
		constexpr Color RED{255, 0, 0};
		constexpr Color GREEN{0, 255, 0};
		constexpr Color BLUE{0, 0, 255};
		constexpr Color YELLOW{255, 255, 0};
		constexpr Color MAGENTA{255, 0, 255};
	}

	struct Pixel : public IRenderSequence {
		Color color_fg;
		Color color_bg;
		wchar_t character;

		Pixel(
			const wchar_t character,
			const Color& fg_color = default_colors::WHITE,
			const Color& bg_color = default_colors::BLACK
		) : color_fg{fg_color}, color_bg{bg_color}, character{character} {
		}

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

} // render