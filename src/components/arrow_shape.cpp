#include "arrow_shape.h"
#include "managers/display_server.h"

ArrowShape::ArrowShape(ArrowShape::Direction p_direction) :
		direction(p_direction) {
	set_size(Vector2i::ZERO);
}

void ArrowShape::update(UNUSED_PARAM float p_delta) {
	if (!is_drawable()) {
		return;
	}
	Vector2i position = get_position();
	Vector2i size = get_size();
	draw_triangle_in_rectangle(position, size, direction, 255, 255, 255);
}

Vector2i ArrowShape::get_minimum_size() const {
	static Vector2i default_size = Vector2i::ZERO;
	switch (direction) {
		case ArrowShape::Direction::UP:
		case ArrowShape::Direction::DOWN:
			default_size = Vector2i(10, 5);
			break;
		case ArrowShape::Direction::LEFT:
		case ArrowShape::Direction::RIGHT:
			default_size = Vector2i(5, 10);
			break;
	}

	return default_size < get_custom_minimum_size() ? get_custom_minimum_size() : default_size;
}

void ArrowShape::draw_triangle_in_rectangle(Vector2i position, Vector2i size, Direction direction, uint8_t p_border_thickness, uint8_t p_fill_intensity, uint8_t p_border_intensity) {
	Vector2i v1, v2, v3;

	switch (direction) {
		case Direction::UP:
			v1 = Vector2i(position.x + size.x / 2, position.y); // Top middle
			v2 = Vector2i(position.x, position.y + size.y); // Bottom left
			v3 = Vector2i(position.x + size.x, position.y + size.y); // Bottom right
			break;
		case Direction::DOWN:
			v1 = Vector2i(position.x, position.y); // Top left
			v2 = Vector2i(position.x + size.x, position.y); // Top right
			v3 = Vector2i(position.x + size.x / 2, position.y + size.y); // Bottom middle
			break;
		case Direction::LEFT:
			v1 = Vector2i(position.x + size.x, position.y); // Top right
			v2 = Vector2i(position.x + size.x, position.y + size.y); // Bottom right
			v3 = Vector2i(position.x, position.y + size.y / 2); // Left middle
			break;
		case Direction::RIGHT:
			v1 = Vector2i(position.x, position.y); // Top left
			v2 = Vector2i(position.x, position.y + size.y); // Bottom left
			v3 = Vector2i(position.x + size.x, position.y + size.y / 2); // Right middle
			break;
	}

	DisplayServer::get_singleton()->draw_triangle(v1, v2, v3, p_border_thickness, p_fill_intensity, p_border_intensity);
}
