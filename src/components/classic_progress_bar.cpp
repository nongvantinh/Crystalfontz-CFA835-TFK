#include "classic_progress_bar.h"
#include "managers/display_server.h"
#include "viewport.h"
#include <iostream>

void ClassicProgressBar::initialize() {
}

void ClassicProgressBar::update(UNUSED_PARAM float p_delta) {
	Vector2i position = get_position();
	Vector2i size = get_size();
	uint8_t fill = 0;
	uint8_t border_intensity = 255;

	// Draw borders.
	DisplayServer::get_singleton()->draw_rectangle(position, size, border_intensity, fill);

	// Fill the spaces based on the ratio between current value and max value.
	fill = 255;
	uint32_t size_x = (float_t(get_value()) / float_t(get_max_value())) * size.x;

	Vector2i fill_bar(size_x, size.y);
	DisplayServer::get_singleton()->draw_rectangle(position, fill_bar, border_intensity, fill);
}
