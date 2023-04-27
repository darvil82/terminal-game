export module render.pixel;

import <cstdint>;
import <string>;
import <sstream>;
import render.sequences;


namespace render {
	
	export struct Color : public IRenderSequence {
		uint8_t r, g, b;

		constexpr Color(uint8_t r, uint8_t g, uint8_t b) :
			r {r}, g {g}, b {b} {
		}

		bool operator==(const Color& other) const {
			return this->r == other.r && this->g == other.g && this->b == other.b;
		}

		std::wstring get_sequence(bool background) const {
			return TerminalSequences::set_color(*this, background);
		}

		std::wstring get_sequence() const override {
			return this->get_sequence(false);
		}
	};


	export namespace default_colors {
		constexpr Color BLACK {0, 0, 0};
		constexpr Color WHITE {255, 255, 255};
		constexpr Color RED {255, 0, 0};
		constexpr Color GREEN {0, 255, 0};
		constexpr Color BLUE {0, 0, 255};
		constexpr Color YELLOW {255, 255, 0};
		constexpr Color MAGENTA {255, 0, 255};
		constexpr Color CYAN {0, 255, 255};
	}

	export struct Pixel : public IRenderSequence {
		Color color_fg;
		Color color_bg;
		wchar_t character;

		Pixel(
			const wchar_t character,
			const Color& fg_color,
			const Color& bg_color
		) : color_fg {fg_color}, color_bg {bg_color}, character {character} { }

		std::wstring get_sequence() const override {
			std::wstringstream buff;
			buff << this->color_fg.get_sequence()
				<< this->color_bg.get_sequence(true)
				<< this->character;
			return buff.str();
		}
	};


} // render