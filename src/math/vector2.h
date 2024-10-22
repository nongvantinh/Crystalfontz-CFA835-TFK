#ifndef VECTOR2_H
#define VECTOR2_H
#include <iostream>

class Vector2 {
public:
	static const Vector2 UP;
	Vector2();
	Vector2(float p_x, float p_y);
	~Vector2();

	Vector2 operator+(const Vector2 &p_v) const;
	void operator+=(const Vector2 &p_v);
	Vector2 operator-(const Vector2 &p_v) const;
	void operator-=(const Vector2 &p_v);
	Vector2 operator*(const Vector2 &p_v1) const;
	Vector2 operator*(float p_rvalue) const;
	void operator*=(float p_rvalue);
	Vector2 operator/(const Vector2 &p_v1) const;
	Vector2 operator/(float p_rvalue) const;
	void operator/=(float p_rvalue);

	bool operator==(const Vector2 &other) const {
		return (x == other.x) && (y == other.y);
	}
	bool operator!=(const Vector2 &other) const {
		return !(*this == other);
	}
	bool operator<(const Vector2 &p_vec2) const { return (x == p_vec2.x) ? (y < p_vec2.y) : (x < p_vec2.x); }
	bool operator>(const Vector2 &p_vec2) const { return (x == p_vec2.x) ? (y > p_vec2.y) : (x > p_vec2.x); }
	friend std::ostream &operator<<(std::ostream &os, const Vector2 &vec);

	float x;
	float y;
};

#endif // VECTOR2_H