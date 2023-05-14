#pragma once

#include <inttypes.h>
#include <string>
#include <sstream>
#include <optional>
#include <functional>
#include "Characters.hpp"
#include "IRenderSequence.hpp"
#include "../utils/Point.hpp"

namespace render {


	struct Color : public IRenderSequence {
		uint8_t r, g, b;

		constexpr Color(uint8_t r, uint8_t g, uint8_t b) :
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
		constexpr Color CYAN {0, 255, 255};
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

		Pixel() : Pixel {L' ', default_colors::WHITE, default_colors::BLACK} { }

		const Pixel& operator=(const Pixel& other);
		bool operator==(const Pixel& other) const;

		std::wstring get_sequence() const override;
	};


} // render