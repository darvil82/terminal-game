#pragma once

#include <functional>

#define ON_CLEANUP(fn_body) utils::Cleanup __cleanup([&] fn_body)

namespace utils {

	class Cleanup {
		std::function<void()> func;

	public:
		Cleanup(std::function<void()> f) : func {f} { }

		~Cleanup() {
			this->func();
		}
	};

} // utils