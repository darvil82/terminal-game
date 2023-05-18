#include "Pixel.hpp"

namespace render {

	const Pixel& Pixel::operator=(const Pixel& other) {
		this->color_fg = other.color_fg;
		this->color_bg = other.color_bg;
		this->character = other.character;
		return *this;
	}

	bool Pixel::operator==(const Pixel& other) const {
		return this->color_fg == other.color_fg
			&& this->color_bg == other.color_bg
			&& this->character == other.character;
	}

	std::wstring Pixel::get_sequence() const {
		std::wstringstream buff;
		buff << this->color_fg.get_sequence()
			<< this->color_bg.get_sequence(true)
			<< this->character;
		return buff.str();
	}


} // render