#include "text.h"
#include "font_string_names.h"
#include "managers/display_server.h"
#include "viewport.h"
#include <iostream>

Text::Text() :
		font(FontStringName(roboto_condensed_12)), text("") {
}

void Text::initialize() {
}

void Text::update(UNUSED_PARAM float p_delta) {
	DisplayServer::get_singleton()->print_text_to_display(get_font(), get_text(), get_position());
}

void Text::set_text(const std::string p_text) {
	text = p_text;
}

std::string Text::get_text() const {
	return text;
}

void Text::set_font(const std::string p_font) {
	font = p_font;
}

std::string Text::get_font() const {
	return font;
}

Vector2i Text::get_minimum_size() const {
	return DisplayServer::get_singleton()->get_required_space_for_text(get_font(), get_text());
}