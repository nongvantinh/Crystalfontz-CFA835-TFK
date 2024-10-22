#ifndef VECTOR2I_H
#define VECTOR2I_H
#include <algorithm>
#include <iostream>

class Vector2i {
public:
	static const Vector2i ZERO;
	static const Vector2i UP;
	Vector2i();
	Vector2i(int p_x, int p_y);
	~Vector2i();

	Vector2i max(const Vector2i &p_vector2) const {
		return Vector2i(std::max(x, p_vector2.x), std::max(y, p_vector2.y));
	}

	Vector2i operator+(const Vector2i &p_v) const;
	void operator+=(const Vector2i &p_v);
	Vector2i operator-(const Vector2i &p_v) const;
	void operator-=(const Vector2i &p_v);
	Vector2i operator*(const Vector2i &p_v1) const;
	Vector2i operator*(int32_t p_rvalue) const;
	void operator*=(int32_t p_rvalue);
	Vector2i operator/(const Vector2i &p_v1) const;
	Vector2i operator/(int32_t p_rvalue) const;
	void operator/=(int32_t p_rvalue);

	bool operator==(const Vector2i &other) const {
		return (x == other.x) && (y == other.y);
	}
	bool operator!=(const Vector2i &other) const {
		return !(*this == other);
	}
	bool operator<(const Vector2i &p_vec2) const { return (x == p_vec2.x) ? (y < p_vec2.y) : (x < p_vec2.x); }
	bool operator>(const Vector2i &p_vec2) const { return (x == p_vec2.x) ? (y > p_vec2.y) : (x > p_vec2.x); }
	friend std::ostream &operator<<(std::ostream &os, const Vector2i &vec);

	int x;
	int y;
};

#endif // VECTOR2I_H