#pragma once

#include <mutex>
#include <thread>

#define SYNCHRONIZED(mutex, body) { std::scoped_lock __lock_(mutex); body }

namespace utils {
	class Thread : public std::thread {
		using std::thread::thread;

	public:
		void set_name(const char *name);
	};
}