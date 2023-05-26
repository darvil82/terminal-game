#pragma once

namespace math {
	/**
	 * @brief Returns the sign of a number
	 * @return 1 if positive, -1 if negative, 0 if zero
	 */
	template<typename T>
	T sign(T val) {
		return (static_cast<T>(0) < val) - (val < static_cast<T>(0));
	}
}