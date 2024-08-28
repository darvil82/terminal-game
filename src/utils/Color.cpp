#include "Color.hpp"
#include "Terminal.hpp"

namespace utils {
	std::string Color::get_sequence(bool background) const {
		return Terminal::set_color(*this, background);
	}

	std::string Color::get_sequence() const {
		return this->get_sequence(false);
	}

	bool Color::operator==(const Color& other) const {
		return this->r == other.r && this->g == other.g && this->b == other.b;
	}
}