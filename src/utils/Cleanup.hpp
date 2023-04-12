#include <functional>

#pragma once

namespace utils {

	class Cleanup {
		std::function<void()> func;

	public:
		Cleanup(std::function<void()> f): func{f} {}

		~Cleanup() {
			this->func();
		}
	};

} // utils