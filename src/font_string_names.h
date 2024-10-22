#ifndef FONT_STRING_NAMES_H
#define FONT_STRING_NAMES_H

#include <string>

class FontStringNames {
public:
	FontStringNames();
	static FontStringNames *get_singleton();
	std::string roboto_condensed_light_9;

	std::string roboto_condensed_regular_8;
	std::string roboto_condensed_regular_9;

	std::string roboto_condensed_11;
	std::string roboto_condensed_12;
	std::string roboto_condensed_13;
	std::string roboto_condensed_14;
	std::string roboto_condensed_15;
	std::string roboto_condensed_16;

	std::string time_new_roman_regular_12;
	std::string time_new_roman_regular_14;
	std::string time_new_roman_regular_16;
	std::string time_new_roman_regular_20;

	std::string time_new_roman_bold_12;
	std::string time_new_roman_bold_14;
	std::string time_new_roman_bold_16;
	std::string time_new_roman_bold_20;
};

#define FontStringName(m_name) FontStringNames::get_singleton()->m_name

#endif // FONT_STRING_NAMES_H