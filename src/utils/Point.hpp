#pragma once

#include <stdint.h>


namespace utils {
	template<typename T = float>
	struct Point {
		T x, y;

		Point(T x = 0, T y = 0) : x {x}, y {y} { }

		template<typename F> operator Point<F>() const {
			return {static_cast<F>(this->x), static_cast<F>(this->y)};
		}

		Point<T> operator+(const Point<T>& other) const {
			return {this->x + other.x, this->y + other.y};
		}

		Point<T> operator+(const T& other) const {
			return {this->x + other, this->y + other};
		}

		Point<T>& operator+=(const Point<T>& other) {
			this->x += other.x;
			this->y += other.y;
			return *this;
		}

		Point<T>& operator+=(const T& other) {
			this->x += other;
			this->y += other;
			return *this;
		}

		Point<T> operator-(const Point<T>& other) const {
			return {this->x - other.x, this->y - other.y};
		}

		Point<T> operator-(const T& other) const {
			return {this->x - other, this->y - other};
		}

		Point<T>& operator-=(const Point<T>& other) {
			this->x -= other.x;
			this->y -= other.y;
			return *this;
		}

		Point<T>& operator-=(const T& other) {
			this->x -= other;
			this->y -= other;
			return *this;
		}

		Point<T> operator*(const Point<T>& other) const {
			return {this->x * other.x, this->y * other.y};
		}

		Point<T> operator*(const T& other) const {
			return {this->x * other, this->y * other};
		}

		Point<T>& operator*=(const Point<T>& other) {
			this->x *= other.x;
			this->y *= other.y;
			return *this;
		}

		Point<T>& operator*=(const T& other) {
			this->x *= other;
			this->y *= other;
			return *this;
		}

		Point<T> operator/(const Point<T>& other) const {
			return {this->x / other.x, this->y / other.y};
		}

		Point<T> operator/(const T& other) const {
			return {this->x / other, this->y / other};
		}

		Point<T>& operator/=(const Point<T>& other) {
			this->x /= other.x;
			this->y /= other.y;
			return *this;
		}

		Point<T>& operator/=(const T& other) {
			this->x /= other;
			this->y /= other;
			return *this;
		}

		bool operator==(const Point<T>& other) const {
			return this->x == other.x && this->y == other.y;
		}
	};
}