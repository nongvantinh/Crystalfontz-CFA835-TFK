#include "core/geometry_shape.h"

GeometryShape::GeometryShape(const ShapeCategory &p_shape_category) :
		shape(p_shape_category) {
	set_size(Vector2i::ZERO);
}

void GeometryShape::update(UNUSED_PARAM float p_delta) {
	if (!is_drawable()) {
		return;
	}
	DisplayServer::get_singleton()->print_shape_to_display(shape, get_position());
}

Vector2i GeometryShape::get_minimum_size() const {
	return DisplayServer::get_singleton()->get_shape_size(shape);
}
