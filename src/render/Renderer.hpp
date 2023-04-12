#pragma once

#include <inttypes.h>
#include <string>

#include "../utils/Point.hpp"
#include "Characters.hpp"

namespace render {
	constexpr const char* ESCAPE_SEQUENCE_START = "\x1b[";

	class IRenderSequence {
		virtual std::wstring get_sequence() const = 0;
	};

	struct Color : public IRenderSequence {
		uint8_t r = 0, g = 0, b = 0;
		bool background = false;

		Color(uint8_t r, uint8_t g, uint8_t b, bool background = false) :
			r{r}, g{g}, b{b}, background{background} {}

		bool operator==(const Color& other) const;

		std::wstring get_sequence() const override;
	};

	struct RPoint : public utils::Point<uint16_t>, IRenderSequence {
		using Point::Point;

		std::wstring get_sequence() const override;
	};

	struct Pixel : public IRenderSequence {
		RPoint pos;
		Color color;
		wchar_t character;

		Pixel(const RPoint& pos, const Color& color, wchar_t character) :
			pos{pos}, color{color}, character{character} {}

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
		void set_pixel(const Pixel& pixel);
		const Pixel& get_pixel(const RPoint pos) const;
		void clear_all();
		void push_buffer();
	};

} // render