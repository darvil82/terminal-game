#pragma once

#include <inttypes.h>
#include <string>
#include <sstream>
#include <optional>
#include <functional>
#include "Characters.hpp"
#include "../utils/Point.hpp"
#include "../utils/Color.hpp"
#include "../utils/ITerminalSequence.hpp"

namespace render {
	struct Pixel : public utils::ITerminalSequence {
		utils::Color color_fg;
		utils::Color color_bg;
		wchar_t character;

		Pixel(
			const wchar_t character,
			const utils::Color& fg_color,
			const utils::Color& bg_color
		) : color_fg {fg_color}, color_bg {bg_color}, character {character} { }

		Pixel() : Pixel {L' ', utils::default_colors::WHITE, utils::default_colors::BLACK} { }

		const Pixel& operator=(const Pixel& other);
		bool operator==(const Pixel& other) const;

		std::wstring get_sequence() const override;
	};


} // render