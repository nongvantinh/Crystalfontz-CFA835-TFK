#include "vector2i.h"

const Vector2i Vector2i::ZERO = Vector2i(0, 0);
const Vector2i Vector2i::UP = Vector2i(0, 1);

std::ostream &operator<<(std::ostream &os, const Vector2i &vec) {
	os << "(" << vec.x << ", " << vec.y << ")";
	return os;
}

Vector2i::Vector2i() :
		x(0), y(0) {
}

Vector2i::Vector2i(int p_x, int p_y) :
		x(p_x), y(p_y) {
}

Vector2i::~Vector2i() {
}

Vector2i Vector2i::operator+(const Vector2i &p_v) const {
	return Vector2i(x + p_v.x, y + p_v.y);
}

void Vector2i::operator+=(const Vector2i &p_v) {
	x += p_v.x;
	y += p_v.y;
}

Vector2i Vector2i::operator-(const Vector2i &p_v) const {
	return Vector2i(x - p_v.x, y - p_v.y);
}

void Vector2i::operator-=(const Vector2i &p_v) {
	x -= p_v.x;
	y -= p_v.y;
}

Vector2i Vector2i::operator*(const Vector2i &p_v1) const {
	return Vector2i(x * p_v1.x, y * p_v1.y);
}

Vector2i Vector2i::operator*(int32_t p_rvalue) const {
	return Vector2i(x * p_rvalue, y * p_rvalue);
}

void Vector2i::operator*=(int32_t p_rvalue) {
	x *= p_rvalue;
	y *= p_rvalue;
}

Vector2i Vector2i::operator/(const Vector2i &p_v1) const {
	return Vector2i(x / p_v1.x, y / p_v1.y);
}

Vector2i Vector2i::operator/(int32_t p_rvalue) const {
	return Vector2i(x / p_rvalue, y / p_rvalue);
}

void Vector2i::operator/=(int32_t p_rvalue) {
	x /= p_rvalue;
	y /= p_rvalue;
}
