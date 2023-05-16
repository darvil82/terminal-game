#include <functional>

template<class... T>
using Predicate = std::function<bool(T...)>;

namespace chrono = std::chrono;
using timestamp = decltype(chrono::steady_clock::now());