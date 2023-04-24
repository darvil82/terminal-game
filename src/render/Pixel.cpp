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

	std::wstring Pixel::get_sequence() const {
		std::wstringstream buff;
		buff << this->color_fg.get_sequence()
			<< this->color_bg.get_sequence(true)
			<< this->character;
		return buff.str();
	}


} // render