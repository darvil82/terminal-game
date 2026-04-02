#include <thread>

#include "Threads.hpp"

namespace utils {
	void Thread::set_name(const char* name) {
		pthread_setname_np(this->native_handle(), name);
	}
}