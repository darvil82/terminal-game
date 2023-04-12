#pragma once

#include <inttypes.h>
#include <string>
#include <optional>

#include "../utils/Point.hpp"
#include "Characters.hpp"

namespace render {
	constexpr const char* ESCAPE_SEQUENCE_START = "\x1b[";

	class IRenderSequence {
		virtual std::wstring get_sequence() const = 0;
	};

	struct Color : public IRenderSequence {
		uint8_t r = 0, g = 0, b = 0;

		constexpr Color(uint8_t r, uint8_t g, uint8_t b) :
			r{r}, g{g}, b{b} {}

		bool operator==(const Color& other) const;

		std::wstring get_sequence(bool background) const;
		std::wstring get_sequence() const override;
	};

	namespace default_colors {
		constexpr Color BLACK { 0, 0 ,0 };
		constexpr Color WHITE { 255, 255, 255 };
	}

	struct RPoint : public utils::Point<uint16_t>, IRenderSequence {
		using Point::Point;

		std::wstring get_sequence() const override;
	};

	struct Pixel : public IRenderSequence {
		Color color_fg;
		Color color_bg;
		wchar_t character;

		Pixel(
			const wchar_t character,
			const Color& fg_color = default_colors::WHITE,
			const Color& bg_color = default_colors::BLACK
		) : color_fg{fg_color}, color_bg{bg_color}, character{character} {}

		std::wstring get_sequence() const override;
	};

	class Renderer {
		uint16_t buffer_width = 50, buffer_height = 50;
		const Pixel*** buffer = nullptr; // pixel matrix

		void free_buff();
		bool is_in_bounds(const render::RPoint& pos) const;

	public:
		Renderer(uint16_t width, uint16_t height);

		~Renderer();

		void resize(uint16_t new_width, uint16_t new_height);
		void set_pixel(const Pixel& pixel, const RPoint& position);
		const Pixel& get_pixel(const RPoint pos) const;
		void clear_all();
		void push_buffer();
	};

} // render