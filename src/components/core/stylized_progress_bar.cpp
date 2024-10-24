#include "stylized_progress_bar.h"

#include <cmath>
#include <cstdint>

void StylizedProgressBar::initialize() {
}

void StylizedProgressBar::set_max_value(uint32_t p_value) {
	ProgressBar::set_max_value(p_value);
}

void StylizedProgressBar::update(UNUSED_PARAM float p_delta) {
	timer += p_delta;
	if (threshold <= timer) {
		timer -= threshold;
		increment_index();
		set_value(current_index);
	}

	Vector2i position = get_position();
	Vector2i size = get_size();

	// Fill the spaces based on the ratio between current value and max value.
	Vector2i shape_size = DisplayServer::get_singleton()->get_shape_size(shape);
	uint32_t space_x = (size.x / (get_max_value() * shape_size.x)) / get_max_value() - 1;

	DisplayServer::get_singleton()->print_shape_to_display(shape, position);

	for (size_t i = 0; i <= current_index; ++i) {
		DisplayServer::get_singleton()->print_shape_to_display(shape, position);
		position.x += shape_size.x + space_x;
	}
}

Vector2i StylizedProgressBar::get_minimum_size() const {
	int32_t max_element = get_max_value();
	Vector2i minsize = Vector2i::ZERO;
	Vector2i min_element_size = DisplayServer::get_singleton()->get_shape_size(shape);

	minsize.x = max_element * min_element_size.x + (max_element - 1) * min_space;
	minsize.y = min_element_size.y;

	return minsize;
}

void StylizedProgressBar::increment_index() {
	current_index = (current_index + 1) % get_max_value();
}

void StylizedProgressBar::decrement_index() {
	current_index = (current_index - 1 + get_max_value()) % get_max_value();
}
