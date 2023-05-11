#include "Pixel.hpp"
#include "TerminalSequences.hpp"

namespace render {

	std::wstring Color::get_sequence(bool background) const {
		return TerminalSequences::set_color(*this, background);
	}

	std::wstring Color::get_sequence() const {
		return this->get_sequence(false);
	}

	bool Color::operator==(const Color& other) const {
		return this->r == other.r && this->g == other.g && this->b == other.b;
	}

	const Pixel& Pixel::operator=(const Pixel& other) {
		this->color_fg = other.color_fg;
		this->color_bg = other.color_bg;
		this->character = other.character;
		return *this;
	}

	std::wstring Pixel::get_sequence() const {
		std::wstringstream buff;
		buff << this->color_fg.get_sequence()
			<< this->color_bg.get_sequence(true)
			<< this->character;
		return buff.str();
	}


} // render