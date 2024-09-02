#include <mutex>

#define SYNCHRONIZED(mutex, body) { std::scoped_lock __lock(mutex); body }
#define SYNCHRONIZED_NAMED(mutex, name, body) { std::scoped_lock __lock##name(mutex); body }