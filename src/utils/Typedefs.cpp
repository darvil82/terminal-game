import <functional>;

export module utils.typedefs;

export template<class... T>
using Predicate = std::function<bool(T...)>;
