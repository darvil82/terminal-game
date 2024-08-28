#pragma once


#include <inttypes.h>
#include <string>
#include <sstream>
#include <optional>
#include <functional>

#include "ITerminalSequence.hpp"


namespace utils {
	class ITerminalSequence;

	struct Color : public ITerminalSequence {
		uint8_t r, g, b;

		constexpr Color(uint8_t r, uint8_t g, uint8_t b) :
			r {r}, g {g}, b {b} {
		}

		bool operator==(const Color& other) const;

		std::string get_sequence(bool background) const;
		std::string get_sequence() const override;
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
}