#include <functional>

template<class... T>
using Predicate = std::function<bool(T...)>;