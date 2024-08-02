#ifndef ARROW_SHAPE_H
#define ARROW_SHAPE_H

#include "component.h"

class ArrowShape : public Component {
public:
	enum class Direction {
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	ArrowShape(ArrowShape::Direction p_direction);
	void update(float p_delta) override;
	void draw_triangle_in_rectangle(Vector2i position, Vector2i size, Direction direction, uint8_t p_border_thickness, uint8_t p_fill_intensity, uint8_t p_border_intensity = 255);
	Vector2i get_minimum_size() const override;

private:
	Direction direction;
};

#endif // ARROW_SHAPE_H