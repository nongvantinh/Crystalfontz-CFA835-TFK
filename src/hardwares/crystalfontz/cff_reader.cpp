#include "cff_reader.h"

#include <iostream>

CFFReader::CFFReader(const std::string &p_filename) {
	file.open(p_filename, std::ios::binary);
	if (!file) {
		throw std::runtime_error("Failed to open file");
	}
	read_font_header();
}

CFFReader::~CFFReader() {
	if (file.is_open()) {
		file.close();
	}
}

std::unique_ptr<FR_Char_t> CFFReader::read_char_data(uint16_t p_charCode) {
	int charDataSize = fontHeader.DataWidth * fontHeader.DataHeight;
	if (charDataSize % 2)
		charDataSize++;
	int charStructSize = (charDataSize / 2) + FR_CHAR_EXTRA_BYTES;

	int charOffset = sizeof(FR_FileHeader_t) +
			(charStructSize * (p_charCode - fontHeader.StartChar));
	file.seekg(charOffset, std::ios::beg);

	auto charData = std::make_unique<FR_Char_t>();
	charData->CharData.resize(charDataSize / 2);

	file.read(reinterpret_cast<char *>(&charData->CharFlag),
			sizeof(charData->CharFlag));
	file.read(reinterpret_cast<char *>(&charData->CharWidth),
			sizeof(charData->CharWidth));
	file.read(reinterpret_cast<char *>(charData->CharData.data()),
			charDataSize / 2);

	return charData;
}

Vector2i CFFReader::get_space_required(const std::string &p_text) {
	Vector2i space(0, fontHeader.DataHeight);

	for (char c : p_text) {
		std::unique_ptr<FR_Char_t> data = read_char_data(c);
		space.x += data->CharWidth;
	}

	return space;
}

void CFFReader::read_font_header() {
	file.read(reinterpret_cast<char *>(&fontHeader), sizeof(FR_FileHeader_t));
	if (file.gcount() != sizeof(FR_FileHeader_t)) {
		throw std::runtime_error("Failed to read font header");
	}
}