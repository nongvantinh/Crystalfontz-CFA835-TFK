#ifndef CFF_READER_H
#define CFF_READER_H

#include <cstdint>
#include <fstream>
#include <memory>
#include <vector>

#include "math/vector2i.h"

#define FR_FileID "CFFF"
#define FR_FileVersion 105
#define FR_CHAR_EXTRA_BYTES 2

#pragma pack(push, 1)
struct FR_FileHeader_t {
	char ID[4];
	uint16_t Version;

	uint8_t DataWidth;
	uint8_t DataHeight;
	uint16_t StartChar;
	uint16_t EndChar;
	uint8_t CharSpaceRight;
	uint8_t CharSpaceBelow;
	uint8_t ScreenSpaceLeft;
	uint8_t ScreenSpaceTop;
	uint8_t Flags;

	char OrigFont[128];
	uint8_t TrimTop;
	uint8_t TrimBottom;
	uint8_t TrimLeft;
	uint8_t TrimRight;
};
#pragma pack(pop)

struct FR_Char_t {
	uint8_t CharFlag;
	uint8_t CharWidth;
	std::vector<uint8_t> CharData;
};

class CFFReader {
public:
	CFFReader(const std::string &p_filename);
	~CFFReader();

	std::unique_ptr<FR_Char_t> read_char_data(uint16_t p_charCode);
	Vector2i get_space_required(const std::string &p_text);
	const FR_FileHeader_t &get_font_header() const { return fontHeader; }

private:
	std::ifstream file;
	FR_FileHeader_t fontHeader;

	void read_font_header();
};

#endif // CFF_READER_H