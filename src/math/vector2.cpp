#include "vector2.h"

static const Vector2 UP = Vector2(0, 1);

std::ostream &operator<<(std::ostream &os, const Vector2 &vec) {
	os << "(" << vec.x << ", " << vec.y << ")";
	return os;
}

Vector2::Vector2() :
		x(0), y(0) {
}

Vector2::Vector2(float p_x, float p_y) :
		x(p_x), y(p_y) {
}

Vector2::~Vector2() {
}

Vector2 Vector2::operator+(const Vector2 &p_v) const {
	return Vector2(x + p_v.x, y + p_v.y);
}

void Vector2::operator+=(const Vector2 &p_v) {
	x += p_v.x;
	y += p_v.y;
}

Vector2 Vector2::operator-(const Vector2 &p_v) const {
	return Vector2(x - p_v.x, y - p_v.y);
}

void Vector2::operator-=(const Vector2 &p_v) {
	x -= p_v.x;
	y -= p_v.y;
}

Vector2 Vector2::operator*(const Vector2 &p_v1) const {
	return Vector2(x * p_v1.x, y * p_v1.y);
}

Vector2 Vector2::operator*(float p_rvalue) const {
	return Vector2(x * p_rvalue, y * p_rvalue);
}

void Vector2::operator*=(float p_rvalue) {
	x *= p_rvalue;
	y *= p_rvalue;
}

Vector2 Vector2::operator/(const Vector2 &p_v1) const {
	return Vector2(x / p_v1.x, y / p_v1.y);
}

Vector2 Vector2::operator/(float p_rvalue) const {
	return Vector2(x / p_rvalue, y / p_rvalue);
}

void Vector2::operator/=(float p_rvalue) {
	x /= p_rvalue;
	y /= p_rvalue;
}
