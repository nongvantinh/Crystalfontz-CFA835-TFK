#ifndef BITMAP_H
#define BITMAP_H

#include "component.h"
#include <cstring>
#include <filesystem>
#include <fstream>
#include <vector>

class Bitmap : public Component {
public:
	Bitmap(const std::string &p_filepath);
	void set_bmp(const std::string &p_filepath);

	Vector2i get_minimum_size() const override;
	void update(float p_delta) override;

private:
	std::vector<std::uint8_t> get_bitmap_data(const std::string &bmpFile, int32_t &p_width, int32_t &p_height);

	std::vector<uint8_t> data;
	int32_t width = 0;
	int32_t height = 0;
};

#endif // BITMAP_H