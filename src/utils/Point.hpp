#pragma once

#include <stdint.h>


namespace utils {
	template<typename T = float>
	struct Point {
		using AxisType = T;

		T x, y;

		constexpr Point(T x, T y) : x {x}, y {y} { }

		template<typename F> constexpr operator Point<F>() const {
			return {static_cast<F>(this->x), static_cast<F>(this->y)};
		}

		constexpr Point<T> operator+(const Point<T>& other) const {
			return {this->x + other.x, this->y + other.y};
		}

		constexpr Point<T> operator+(const T& other) const {
			return {this->x + other, this->y + other};
		}

		constexpr Point<T>& operator+=(const Point<T>& other) {
			this->x += other.x;
			this->y += other.y;
			return *this;
		}

		constexpr Point<T>& operator+=(const T& other) {
			this->x += other;
			this->y += other;
			return *this;
		}

		constexpr Point<T> operator-(const Point<T>& other) const {
			return {this->x - other.x, this->y - other.y};
		}

		constexpr Point<T> operator-(const T& other) const {
			return {this->x - other, this->y - other};
		}

		constexpr Point<T>& operator-=(const Point<T>& other) {
			this->x -= other.x;
			this->y -= other.y;
			return *this;
		}

		constexpr Point<T>& operator-=(const T& other) {
			this->x -= other;
			this->y -= other;
			return *this;
		}

		constexpr Point<T> operator*(const Point<T>& other) const {
			return {this->x * other.x, this->y * other.y};
		}

		constexpr Point<T> operator*(const T& other) const {
			return {this->x * other, this->y * other};
		}

		constexpr Point<T>& operator*=(const Point<T>& other) {
			this->x *= other.x;
			this->y *= other.y;
			return *this;
		}

		constexpr Point<T>& operator*=(const T& other) {
			this->x *= other;
			this->y *= other;
			return *this;
		}

		constexpr Point<T> operator/(const Point<T>& other) const {
			return {this->x / other.x, this->y / other.y};
		}

		constexpr Point<T> operator/(const T& other) const {
			return {this->x / other, this->y / other};
		}

		constexpr Point<T>& operator/=(const Point<T>& other) {
			this->x /= other.x;
			this->y /= other.y;
			return *this;
		}

		constexpr Point<T>& operator/=(const T& other) {
			this->x /= other;
			this->y /= other;
			return *this;
		}

		constexpr bool operator==(const Point<T>& other) const {
			return this->x == other.x && this->y == other.y;
		}
	};


	using SPoint = Point<int32_t>;
	using UPoint = Point<uint32_t>;
	using FPoint = Point<float>;
}