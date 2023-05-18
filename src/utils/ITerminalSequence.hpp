#pragma once

#include <cstdint>
#include <string>
#include "Color.hpp"
#include "Point.hpp"

namespace utils {

	class ITerminalSequence {
		virtual std::wstring get_sequence() const = 0;
	};
}