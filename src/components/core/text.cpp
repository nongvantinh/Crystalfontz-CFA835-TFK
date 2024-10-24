#include "core/text.h"

Text::Text() :
		font(Font::ROBOTO_REGULAR), font_size(15), text("") {
}

Text::Text(const std::string &p_text) :
		Text() {
	text = p_text;
}

void Text::initialize() {
}

void Text::update(UNUSED_PARAM float p_delta) {
	DisplayServer::get_singleton()->print_text_to_display(get_font(), get_font_size(), get_text(), get_position());
}

void Text::set_text(const std::string &p_text) {
	text = p_text;
}

std::string Text::get_text() const {
	return text;
}

void Text::set_font(const Font &p_font) {
	font = p_font;
}

Font Text::get_font() const {
	return font;
}

void Text::set_font_size(const int32_t &p_size) {
	font_size = p_size;
}

int32_t Text::get_font_size() const {
	return font_size;
}

Vector2i Text::get_minimum_size() const {
	return DisplayServer::get_singleton()->get_required_space_for_text(get_font(), get_font_size(), get_text());
}