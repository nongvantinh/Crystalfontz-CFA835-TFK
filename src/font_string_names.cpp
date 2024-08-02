#include "font_string_names.h"

FontStringNames *FontStringNames::get_singleton() {
	static FontStringNames self;
	return &self;
}

FontStringNames::FontStringNames() {
	roboto_condensed_light_9 = "roboto_condensed_light_9.cff";

	roboto_condensed_regular_8 = "roboto_condensed_regular_8.cff";
	roboto_condensed_regular_9 = "roboto_condensed_regular_9.cff";

	roboto_condensed_11 = "roboto_condensed_11.cff";
	roboto_condensed_12 = "roboto_condensed_12.cff";
	roboto_condensed_13 = "roboto_condensed_13.cff";
	roboto_condensed_14 = "roboto_condensed_14.cff";
	roboto_condensed_15 = "roboto_condensed_15.cff";
	roboto_condensed_16 = "roboto_condensed_16.cff";

	time_new_roman_regular_12 = "time_new_roman_regular_12.cff";
	time_new_roman_regular_14 = "time_new_roman_regular_14.cff";
	time_new_roman_regular_16 = "time_new_roman_regular_16.cff";
	time_new_roman_regular_20 = "time_new_roman_regular_20.cff";

	time_new_roman_bold_12 = "time_new_roman_bold_12.cff";
	time_new_roman_bold_14 = "time_new_roman_bold_14.cff";
	time_new_roman_bold_16 = "time_new_roman_bold_16.cff";
	time_new_roman_bold_20 = "time_new_roman_bold_20.cff";
}
