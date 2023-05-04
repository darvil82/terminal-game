#pragma once

namespace math {
	template<typename T>
	T sign(T val) {
		return (static_cast<T>(0) < val) - (val < static_cast<T>(0));
	}
}