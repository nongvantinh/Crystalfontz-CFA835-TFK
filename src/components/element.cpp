#include "element.h"
#include "managers/display_server.h"
#include "viewport.h"
#include <iostream>

void Element::initialize() {
}

void Element::update(UNUSED_PARAM float p_delta) {
	Vector2i position = get_position();
	Vector2i size = get_size();

	uint8_t intensity = 0;
	uint8_t border_intensity = 255;

	DisplayServer::get_singleton()->draw_rectangle(position, size, border_intensity, intensity);
}
