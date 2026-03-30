#include <mutex>

#define SYNCHRONIZED(mutex, body) { std::scoped_lock __lock_(mutex); body }