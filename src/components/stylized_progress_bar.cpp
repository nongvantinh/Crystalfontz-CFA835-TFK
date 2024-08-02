#include "stylized_progress_bar.h"
#include "managers/display_server.h"
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
	uint8_t normal_intensity = 125;
	uint8_t hightlight_intensity = 255;
	const uint32_t space_between = 1;

	// Fill the spaces based on the ratio between current value and max value.
	uint32_t size_x = size.x / (get_max_value() + space_between);
	uint32_t space_x = size_x / (get_max_value() - space_between);

	for (size_t i = 0; i != get_max_value(); ++i) {
		Vector2i fill_bar(size_x, size.y);
		uint8_t intensity = i == get_value() ? hightlight_intensity : normal_intensity;
		DisplayServer::get_singleton()->draw_rectangle(position, fill_bar, intensity, intensity);
		position.x += size_x + space_x;
	}
}

Vector2i StylizedProgressBar::get_minimum_size() const {
	int32_t max_element = get_max_value();
	Vector2i minsize = Vector2i::ZERO;

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
