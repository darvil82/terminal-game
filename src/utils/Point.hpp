#pragma once

#include <stdint.h>

namespace utils {
	template<typename T = float>
	struct Point {
		T x = 0, y = 0;

		Point(T x, T y) : x{x}, y{y} {}

		template<typename F> operator Point<F>() const {
			return { (F)this->x, (F)this->y };
		}

		Point<T> operator+(const Point<T>& other) const {
			return { this->x + other.x, this->y + other.y };
		}

		void operator+=(const Point<T>& other) {
			this->x += other.x;
			this->y += other.y;
		}

		template<typename F> Point<F> operator*(const F& other) const {
			return { this->x * other, this->y * other };
		}

		bool operator==(const Point<T>& other) const {
			return this->x == other.x && this->y == other.y;
		}
	};
}