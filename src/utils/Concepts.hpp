#pragma once

#include <concepts>

template<class C, class B>
concept Extends = std::is_base_of_v<B, C>;