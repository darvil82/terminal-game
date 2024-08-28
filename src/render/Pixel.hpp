#pragma once

#include <inttypes.h>
#include <string>
#include "../utils/Color.hpp"
#include "../utils/ITerminalSequence.hpp"
#include "../render/Characters.hpp"


namespace render {
	struct Pixel : public utils::ITerminalSequence {
		utils::Color color_fg;
		utils::Color color_bg;
		std::string character;

		Pixel(
			const std::string character,
			const utils::Color& fg_color,
			const utils::Color& bg_color
		) : color_fg {fg_color}, color_bg {bg_color}, character {character} { }

		Pixel() : Pixel {render::default_characters::SPACE, utils::default_colors::WHITE, utils::default_colors::BLACK} { }

		const Pixel& operator=(const Pixel& other);
		bool operator==(const Pixel& other) const;

		std::string get_sequence() const override;
	};


} // render