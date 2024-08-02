#include "progress_bar.h"
#include "managers/display_server.h"
#include "viewport.h"
#include <iostream>

void ProgressBar::set_value(uint32_t p_value) {
	value = p_value;
}

uint32_t ProgressBar::get_value() const {
	return value;
}

void ProgressBar::set_max_value(uint32_t p_value) {
	max_value = p_value;
}

uint32_t ProgressBar::get_max_value() const {
	return max_value;
}