#pragma once

#include <string>


class IRenderSequence {
	virtual std::wstring get_sequence() const = 0;
};
