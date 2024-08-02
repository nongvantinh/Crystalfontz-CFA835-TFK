#include "bitmap.h"

#include "display_server.h"

Bitmap::Bitmap(const std::string &p_filepath) {
	set_bmp(p_filepath);
}

void Bitmap::set_bmp(const std::string &p_filepath) {
	data = get_bitmap_data(p_filepath, width, height);
}

std::vector<std::uint8_t> Bitmap::get_bitmap_data(const std::string &bmpFile, int32_t &p_width, int32_t &p_height) {
	u_int8_t *rawDataToSend = nullptr;
	u_int32_t rawDataToSendSize = 0;
	uint32_t rawSize = 0;
	if (std::filesystem::exists(bmpFile)) {
		FILE *fp = fopen(bmpFile.c_str(), "r");
		if (fp == NULL) {
			throw std::runtime_error("[lcd]: Can not open %" + bmpFile);
			fflush(fp);
			fclose(fp);
			return std::vector<std::uint8_t>('\0');
		}

		// go to raw data offset address
		fseek(fp, 0xA, SEEK_SET);
		int rawOffset = 0;
		if (fread(&rawOffset, 1, sizeof(int), fp) != sizeof(int)) {
			throw std::runtime_error("[lcd]: Can not read raw offset address of %" + bmpFile);
			fflush(fp);
			fclose(fp);
			return std::vector<std::uint8_t>('\0');
		}

		// go to sizw of info header block
		fseek(fp, 0xE, SEEK_SET);
		int dibSize = 0;
		if (fread(&dibSize, 1, sizeof(int), fp) != sizeof(int)) {
			throw std::runtime_error("[lcd]: Can not read dib size of %" + bmpFile);
			fflush(fp);
			fclose(fp);
			return std::vector<std::uint8_t>('\0');
		}

		// go to width of image
		fseek(fp, 0x12, SEEK_SET);
		if (fread(&p_width, 1, sizeof(int), fp) != sizeof(int)) {
			throw std::runtime_error("[lcd]: Can not read image width of %" + bmpFile);
			fflush(fp);
			fclose(fp);
			return std::vector<std::uint8_t>('\0');
		}

		// go to height of image
		fseek(fp, 0x16, SEEK_SET);
		if (fread(&p_height, 1, sizeof(int), fp) != sizeof(int)) {
			throw std::runtime_error("[lcd]: Can not read image height of %" + bmpFile);
			fflush(fp);
			fclose(fp);
			return std::vector<std::uint8_t>('\0');
		}

		// go to bit per pixel
		fseek(fp, 0x1C, SEEK_SET);
		auto bitsPerPixel = 0;
		if (fread(&bitsPerPixel, 1, sizeof(int), fp) != sizeof(int)) {
			throw std::runtime_error("[lcd]: Can not read pits per pixel of %" + bmpFile);
			fflush(fp);
			fclose(fp);
			return std::vector<std::uint8_t>('\0');
		}

		rawDataToSendSize = std::abs(p_width * p_height);

		// go to raw data size
		fseek(fp, 0x22, SEEK_SET);
		if (fread(&rawSize, 1, sizeof(int), fp) != sizeof(int)) {
			throw std::runtime_error("[lcd]: Can not read raw size of %" + bmpFile);
			fflush(fp);
			fclose(fp);
			return std::vector<std::uint8_t>('\0');
		}
		if (rawSize == 0) {
			rawSize = std::abs((p_width + (p_width % 4 != 0 ? 4 - p_width % 4 : 0)) * p_height);
		}

		// go to raw data
		fseek(fp, rawOffset, SEEK_SET);
		u_char buffer[rawSize];
		memset(buffer, 0, rawSize);
		if (fread(buffer, 1, rawSize, fp) != rawSize) {
			throw std::runtime_error("[lcd]: Can not read raw data of %" + bmpFile);
			fflush(fp);
			fclose(fp);
			return std::vector<std::uint8_t>('\0');
		}

		// go to color palette
		fseek(fp, 0xD + dibSize, SEEK_SET);
		uint32_t paletteSize = 4 * (2 << (bitsPerPixel - 1));
		u_char palette[paletteSize];
		memset(palette, 0, paletteSize);
		if (fread(palette, 1, paletteSize, fp) != paletteSize) {
			throw std::runtime_error("[lcd]: Can not read palette of %" + bmpFile);
			fflush(fp);
			fclose(fp);
			return std::vector<std::uint8_t>('\0');
		}

		fflush(fp);
		fclose(fp);
		fp = 0;

		rawDataToSend = (u_int8_t *)malloc(rawDataToSendSize);
		memset(rawDataToSend, 0, rawDataToSendSize);
		int pxIndex = 0;
		// If the number of bytes matching a row (scanline) in the image is not divisible by 4,
		// the line is padded with one to three additional bytes of unspecified value (not necessarily 0) so
		// that the next row will start on a multiple of 4 byte location in memory or in the file.
		// (the total number of bytes in a row can be calculated as the image size/bitmap height in pixels)
		// Following these rules there are several ways to store the pixel data depending on the
		// color depth and the compression type of the bitmap.
		for (int heightUpSizeDown = std::abs(p_height) - 1; heightUpSizeDown >= 0; heightUpSizeDown--) {
			auto heightGeneric = heightUpSizeDown;
			if (p_height < 0) {
				heightGeneric = std::abs(heightUpSizeDown + 1 + p_height);
			}
			auto imageWidthPadding = p_width + (p_width % 4 != 0 ? 4 - p_width % 4 : 0);
			for (auto w = 0; w < imageWidthPadding; w++) {
				if (w < p_width) {
					unsigned int paletteIndex = (buffer[pxIndex]) * 4;
					u_char r = palette[paletteIndex];
					u_char g = palette[paletteIndex + 1];
					u_char b = palette[paletteIndex + 2];
					rawDataToSend[w + p_width * heightGeneric] = (u_int8_t)(0.2989 * r + 0.5870 * g + 0.1140 * b);
				}
				pxIndex++;
			}
		}
	}
	return std::vector<u_int8_t>(rawDataToSend, rawDataToSend + rawDataToSendSize);
}

Vector2i Bitmap::get_minimum_size() const {
	return Vector2i(width, height);
}

void Bitmap::update(UNUSED_PARAM float p_delta) {
	if (!is_drawable()) {
		return;
	}

	DisplayServer::get_singleton()->print_bmp_to_display(data, get_position(), get_size());
}