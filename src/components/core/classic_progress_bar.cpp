#include "classic_progress_bar.h"

#include "viewport.h"
#include <iostream>

void ClassicProgressBar::initialize() {
}

void ClassicProgressBar::update(UNUSED_PARAM float p_delta) {
	Vector2i position = get_position();
	Vector2i size = get_size();
	uint32_t size_x = (float_t(get_value()) / float_t(get_max_value())) * size.x;
	Vector2i fill_bar(size_x, size.y);
	DisplayServer::get_singleton()->print_shape_to_display(ShapeCategory::ARROW_RIGHT, position);
}
