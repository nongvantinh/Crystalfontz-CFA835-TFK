#include "cfa835.h"

#include "hardwares/input.h"
#include "main.h"
#include "managers/device_manager.h"
#include "math/vector2i.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::uint8_t> get_bitmap_data(const std::string &bmpFile, int32_t &p_width, int32_t &p_height) {
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

const std::string CFA835::TEST_CASE_TAG = "[CFA835]";
const std::string CFA835::DEVICE_IDENTIFIER = "CFA835";
const uint8_t CFA835::MAX_PACKAGE_SIZE = 124;
const uint8_t CFA835::MIN_PACKAGE_SIZE = 4;
const uint8_t CFA835::MAX_RETRY = 3;
const uint8_t CFA835::SCREEN_WIDTH = 244;
const uint8_t CFA835::SCREEN_HEIGHT = 68;

CFA835::InterfaceOption operator|(CFA835::InterfaceOption lhs, CFA835::InterfaceOption rhs) {
	using T = std::underlying_type_t<CFA835::InterfaceOption>;
	return static_cast<CFA835::InterfaceOption>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

CFA835::CFA835(std::string p_device) :
		CrystalfontzDevice(p_device), io_context(), serial(io_context, p_device), packages(), is_file_opened(false) {
	set_screen_size(Vector2i(SCREEN_WIDTH, SCREEN_HEIGHT));
	serial.set_option(boost::asio::serial_port_base::baud_rate(115200));
	serial.set_option(boost::asio::serial_port_base::character_size(8));
	serial.set_option(boost::asio::serial_port_base::flow_control(
			boost::asio::serial_port_base::flow_control::none));
	serial.set_option(boost::asio::serial_port_base::parity(
			boost::asio::serial_port_base::parity::none));
	serial.set_option(boost::asio::serial_port_base::stop_bits(
			boost::asio::serial_port_base::stop_bits::one));
}

CFA835::~CFA835() {
	serial.close();
	if (receive_thread.joinable()) {
		receive_thread.join();
	}
}

const uint16_t CFA835::CRCTable[] = {
	0x00000, 0x01189, 0x02312, 0x0329B, 0x04624, 0x057AD, 0x06536, 0x074BF,
	0x08C48, 0x09DC1, 0x0AF5A, 0x0BED3, 0x0CA6C, 0x0DBE5, 0x0E97E, 0x0F8F7,
	0x01081, 0x00108, 0x03393, 0x0221A, 0x056A5, 0x0472C, 0x075B7, 0x0643E,
	0x09CC9, 0x08D40, 0x0BFDB, 0x0AE52, 0x0DAED, 0x0CB64, 0x0F9FF, 0x0E876,
	0x02102, 0x0308B, 0x00210, 0x01399, 0x06726, 0x076AF, 0x04434, 0x055BD,
	0x0AD4A, 0x0BCC3, 0x08E58, 0x09FD1, 0x0EB6E, 0x0FAE7, 0x0C87C, 0x0D9F5,
	0x03183, 0x0200A, 0x01291, 0x00318, 0x077A7, 0x0662E, 0x054B5, 0x0453C,
	0x0BDCB, 0x0AC42, 0x09ED9, 0x08F50, 0x0FBEF, 0x0EA66, 0x0D8FD, 0x0C974,
	0x04204, 0x0538D, 0x06116, 0x0709F, 0x00420, 0x015A9, 0x02732, 0x036BB,
	0x0CE4C, 0x0DFC5, 0x0ED5E, 0x0FCD7, 0x08868, 0x099E1, 0x0AB7A, 0x0BAF3,
	0x05285, 0x0430C, 0x07197, 0x0601E, 0x014A1, 0x00528, 0x037B3, 0x0263A,
	0x0DECD, 0x0CF44, 0x0FDDF, 0x0EC56, 0x098E9, 0x08960, 0x0BBFB, 0x0AA72,
	0x06306, 0x0728F, 0x04014, 0x0519D, 0x02522, 0x034AB, 0x00630, 0x017B9,
	0x0EF4E, 0x0FEC7, 0x0CC5C, 0x0DDD5, 0x0A96A, 0x0B8E3, 0x08A78, 0x09BF1,
	0x07387, 0x0620E, 0x05095, 0x0411C, 0x035A3, 0x0242A, 0x016B1, 0x00738,
	0x0FFCF, 0x0EE46, 0x0DCDD, 0x0CD54, 0x0B9EB, 0x0A862, 0x09AF9, 0x08B70,
	0x08408, 0x09581, 0x0A71A, 0x0B693, 0x0C22C, 0x0D3A5, 0x0E13E, 0x0F0B7,
	0x00840, 0x019C9, 0x02B52, 0x03ADB, 0x04E64, 0x05FED, 0x06D76, 0x07CFF,
	0x09489, 0x08500, 0x0B79B, 0x0A612, 0x0D2AD, 0x0C324, 0x0F1BF, 0x0E036,
	0x018C1, 0x00948, 0x03BD3, 0x02A5A, 0x05EE5, 0x04F6C, 0x07DF7, 0x06C7E,
	0x0A50A, 0x0B483, 0x08618, 0x09791, 0x0E32E, 0x0F2A7, 0x0C03C, 0x0D1B5,
	0x02942, 0x038CB, 0x00A50, 0x01BD9, 0x06F66, 0x07EEF, 0x04C74, 0x05DFD,
	0x0B58B, 0x0A402, 0x09699, 0x08710, 0x0F3AF, 0x0E226, 0x0D0BD, 0x0C134,
	0x039C3, 0x0284A, 0x01AD1, 0x00B58, 0x07FE7, 0x06E6E, 0x05CF5, 0x04D7C,
	0x0C60C, 0x0D785, 0x0E51E, 0x0F497, 0x08028, 0x091A1, 0x0A33A, 0x0B2B3,
	0x04A44, 0x05BCD, 0x06956, 0x078DF, 0x00C60, 0x01DE9, 0x02F72, 0x03EFB,
	0x0D68D, 0x0C704, 0x0F59F, 0x0E416, 0x090A9, 0x08120, 0x0B3BB, 0x0A232,
	0x05AC5, 0x04B4C, 0x079D7, 0x0685E, 0x01CE1, 0x00D68, 0x03FF3, 0x02E7A,
	0x0E70E, 0x0F687, 0x0C41C, 0x0D595, 0x0A12A, 0x0B0A3, 0x08238, 0x093B1,
	0x06B46, 0x07ACF, 0x04854, 0x059DD, 0x02D62, 0x03CEB, 0x00E70, 0x01FF9,
	0x0F78F, 0x0E606, 0x0D49D, 0x0C514, 0x0B1AB, 0x0A022, 0x092B9, 0x08330,
	0x07BC7, 0x06A4E, 0x058D5, 0x0495C, 0x03DE3, 0x02C6A, 0x01EF1, 0x00F78
};

bool CFA835::initialize() {
	discard_initial_data();

	static std::map<KeyActivity, KeyCode> key_press_map{
		{ KeyActivity::KEY_UP_PRESS, KeyCode::UP },
		{ KeyActivity::KEY_DOWN_PRESS, KeyCode::DOWN },
		{ KeyActivity::KEY_LEFT_PRESS, KeyCode::LEFT },
		{ KeyActivity::KEY_RIGHT_PRESS, KeyCode::RIGHT },
		{ KeyActivity::KEY_ENTER_PRESS, KeyCode::ENTER },
		{ KeyActivity::KEY_EXIT_PRESS, KeyCode::CANCEL }
	};

	static std::map<KeyActivity, KeyCode> key_release_map{
		{ KeyActivity::KEY_UP_RELEASE, KeyCode::UP },
		{ KeyActivity::KEY_DOWN_RELEASE, KeyCode::DOWN },
		{ KeyActivity::KEY_LEFT_RELEASE, KeyCode::LEFT },
		{ KeyActivity::KEY_RIGHT_RELEASE, KeyCode::RIGHT },
		{ KeyActivity::KEY_ENTER_RELEASE, KeyCode::ENTER },
		{ KeyActivity::KEY_EXIT_RELEASE, KeyCode::CANCEL }
	};

	on_key_activity_event = [this](KeyActivity p_key_activity) {
		if (key_press_map.find(p_key_activity) != key_press_map.end()) {
			Input::get_singleton()->parse_input_event(std::make_shared<InputEvent>(key_press_map[p_key_activity], true));
		} else if (key_release_map.find(p_key_activity) != key_release_map.end()) {
			Input::get_singleton()->parse_input_event(std::make_shared<InputEvent>(key_release_map[p_key_activity], false));
		} else {
			std::cout << "Key event happened but does not exist in the map.\n";
		}
	};

	receive_thread = std::thread(&CFA835::read_package, this, 5);
	if (receive_thread.joinable()) {
		receive_thread.detach();
	}
	return run_test(this, TEST_CASE_TAG);
}

void CFA835::discard_initial_data() {
	boost::asio::deadline_timer timer(io_context);
	boost::system::error_code ec;
	std::vector<uint8_t> buffer(1024);

	while (true) {
		timer.expires_from_now(boost::posix_time::milliseconds(100));
		timer.async_wait([this](const boost::system::error_code &error) {
			if (!error) {
				serial.cancel();
			}
		});

		bool data_discarded = false;
		boost::asio::async_read(serial, boost::asio::buffer(buffer),
				[this, &ec, &data_discarded, &timer](boost::system::error_code error, std::size_t bytes_transferred) {
					timer.cancel();
					if (error) {
						if (error != boost::asio::error::operation_aborted) {
							ec = error;
						}
					} else {
						data_discarded = bytes_transferred > 0;
					}
				});

		io_context.run();
		io_context.reset();

		if (!data_discarded || ec) {
			break;
		}
	}

	// Cancel the timer if it's still running
	timer.cancel();
	io_context.reset();
}

uint8_t CFA835::read_uint8_t(boost::system::error_code &ec, int p_timeout_seconds) {
	boost::asio::deadline_timer timer(io_context);
	uint8_t byte;

	timer.expires_from_now(boost::posix_time::seconds(p_timeout_seconds));
	timer.async_wait([this, &ec](const boost::system::error_code &error) {
		if (!error) {
			serial.cancel();
		}
		if (ec) {
			return;
		}
	});

	boost::asio::async_read(serial, boost::asio::buffer(&byte, 1),
			[&ec, &timer, this](boost::system::error_code error, std::size_t) {
				timer.cancel();
				if (error) {
					ec = error;
				}
			});

	io_context.run();
	io_context.reset();

	return byte;
}

uint16_t CFA835::read_uint16(boost::system::error_code &ec, int p_timeout_seconds) {
	uint8_t low_byte = read_uint8_t(ec, p_timeout_seconds);
	if (ec) {
		return 0;
	}

	uint8_t high_byte = read_uint8_t(ec, p_timeout_seconds);
	if (ec) {
		return 0;
	}

	return (static_cast<uint16_t>(high_byte) << 8) | low_byte;
}

uint8_t CFA835::send_message(CFA835::CommandCode p_type, const std::vector<uint8_t> &p_data) {
	std::scoped_lock lock{ packages_mutex };

	std::vector<uint8_t> buffer(2);
	buffer[0] = static_cast<uint8_t>(p_type);
	buffer[1] = static_cast<uint8_t>(p_data.size());
	if (p_data.size()) {
		buffer.insert(buffer.end(), p_data.begin(), p_data.end());
	}

	uint16_t crc = get_crc(buffer.begin(), buffer.end());
	buffer.push_back(crc & 0xFF);
	buffer.push_back((crc >> 8) & 0xFF);

	assert(buffer.size() <= MAX_PACKAGE_SIZE);

	boost::asio::write(serial, boost::asio::buffer(buffer));
	// {
	// 	std::cout << "SEND message type: " << static_cast<int>(buffer[0]) << std::endl;
	// 	std::cout << "Sent contents: ";
	// 	for (std::size_t i = 0; i < buffer.size(); ++i) {
	// 		std::cout << static_cast<int>(buffer[i]) << " ";
	// 	}
	// 	std::cout << std::endl;
	// 	for (std::size_t i = 0; i < p_data.size(); ++i) {
	// 		std::cout << p_data[i];
	// 	}
	// 	std::cout << std::endl;
	// 	std::cout << "CRC: " << crc << std::endl;
	// }
	return buffer.size();
}

uint8_t CFA835::send_data(const std::vector<uint8_t> &p_data) {
	std::scoped_lock lock{ packages_mutex };
	boost::asio::write(serial, boost::asio::buffer(p_data));
	// {
	// 	std::cout << "SEND message type: " << static_cast<int>(buffer[0]) << std::endl;
	// 	std::cout << "Sent contents: ";
	// 	for (std::size_t i = 0; i < buffer.size(); ++i) {
	// 		std::cout << static_cast<int>(buffer[i]) << " ";
	// 	}
	// 	std::cout << std::endl;
	// 	for (std::size_t i = 0; i < p_data.size(); ++i) {
	// 		std::cout << p_data[i];
	// 	}
	// 	std::cout << std::endl;
	// 	std::cout << "CRC: " << crc << std::endl;
	// }
	return p_data.size();
}

void CFA835::read_package(int p_timeout_seconds) {
	while (true) {
		boost::system::error_code ec;

		uint8_t command = read_uint8_t(ec, p_timeout_seconds);
		if (ec) {
			if (ec == boost::asio::error::operation_aborted) {
				// std::cerr << "Timeout waiting for command byte." << std::endl;
				continue;
			} else {
				std::cerr << "Error reading command byte: " << ec.message() << std::endl;
				return;
			}
		}

		uint8_t data_length = read_uint8_t(ec, p_timeout_seconds);
		if (ec) {
			if (ec == boost::asio::error::operation_aborted) {
				// std::cerr << "Timeout waiting for command byte." << std::endl;
				continue;
			} else {
				std::cerr << "Error reading data length byte: " << ec.message() << std::endl;
				return;
			}
		}

		std::vector<uint8_t> data(data_length);
		if (data_length > 0) {
			for (size_t i = 0; i < data_length; ++i) {
				data[i] = read_uint8_t(ec, p_timeout_seconds);
				if (ec) {
					std::cerr << "Error reading data bytes: " << ec.message() << std::endl;
					return;
				}
			}
		}

		uint16_t crc = read_uint16(ec, p_timeout_seconds);
		if (ec) {
			if (ec == boost::asio::error::operation_aborted) {
				std::cerr << "Timeout waiting for command byte." << std::endl;
				continue;
			} else {
				std::cerr << "Error reading CRC bytes: " << ec.message() << std::endl;
				return;
			}
		}

		process_package(static_cast<CommandCode>(command), data, crc);
	}
}

void CFA835::process_package(CommandCode command, const std::vector<uint8_t> &data, uint16_t crc) {
	// std::cout << "Received package:" << std::endl;
	// std::cout << "  Command: " << static_cast<int>(command) << std::endl;
	// std::cout << "  Data Length: " << data.size() << std::endl;
	// std::cout << "  Data: ";
	// for (const auto &byte : data) {
	// 	std::cout << static_cast<int>(byte) << " ";
	// }
	// std::cout << std::endl;
	// std::cout << "  CRC: " << crc << std::endl;
	std::vector<uint8_t> crc_check{ static_cast<uint8_t>(command), static_cast<uint8_t>(data.size()) };
	crc_check.insert(crc_check.end(), data.begin(), data.end());
	uint16_t computed_crc = get_crc(crc_check.begin(), crc_check.end());
	if (crc != computed_crc) {
		std::cout << "This packet has been corrupted, [CRC in packet = " << crc << "]"
				  << "[computed CRC = " << computed_crc << "]" << std::endl;
		return;
	}

	CFA835::CommandCode type = static_cast<CFA835::CommandCode>(int(command) & ~int(CFA835::CommandCode::RESPONSE_MESSAGE));
	if (type == CFA835::CommandCode::KEY_ACTIVITY) {
		if (on_key_activity_event) {
			std::thread([this, data]() {
				on_key_activity_event(CFA835::KeyActivity(data[0]));
			}).detach();
		}

		return;
	}

	{
		std::scoped_lock<std::mutex> lock{ packages_mutex };
		packages.emplace(type, data);
	}

	packages_cv.notify_one();
}

std::vector<uint8_t> CFA835::receive_message(CFA835::CommandCode p_command, size_t p_expected_size) {
	std::unique_lock<std::mutex> lock(packages_mutex);
	packages_cv.wait(lock, [this, p_command, p_expected_size] {
		if (!packages.empty()) {
			auto &front = packages.front();
			if (front.first == p_command && front.second.size() == p_expected_size) {
				return true;
			}
		}
		return false;
	});

	auto package = packages.front();
	packages.pop();
	return package.second;
}

uint16_t CFA835::get_crc(std::vector<uint8_t>::const_iterator p_begin, std::vector<uint8_t>::const_iterator p_end) {
	uint16_t new_crc = 0xFFFF;

	while (p_begin != p_end) {
		new_crc = (new_crc >> 8) ^ CRCTable[(new_crc ^ *p_begin++) & 0xFF];
	}

	new_crc = ~new_crc;
	return new_crc;
}

std::string CFA835::ping(const std::string &p_message) {
	CFA835::CommandCode command = CFA835::CommandCode::PING;
	std::vector<uint8_t> data(p_message.begin(), p_message.end());
	send_message(command, data);
	std::vector<uint8_t> result = receive_message(command, data.size());
	return std::string(result.begin(), result.end());
}

std::string CFA835::get_hardware_and_firmware_info() {
	CFA835::CommandCode command = CFA835::CommandCode::GET_MODULE_INFOMATION;
	uint8_t hardware_and_firmware_code = 0;
	const uint8_t hardware_and_firmware_size = 16;

	std::vector<uint8_t> data{ hardware_and_firmware_code };

	send_message(command, data);
	std::vector<uint8_t> result = receive_message(command, hardware_and_firmware_size);
	return std::string(result.begin(), result.end());
}

std::string CFA835::get_serial_number() {
	CFA835::CommandCode command = CFA835::CommandCode::GET_MODULE_INFOMATION;
	uint8_t serial_code = 1;
	const uint8_t serial_number_size = 17;

	std::vector<uint8_t> data{ serial_code };

	send_message(command, data);
	std::vector<uint8_t> result = receive_message(command, serial_number_size);
	return std::string(result.begin(), result.end());
}

bool CFA835::write_user_flash_area(const std::vector<uint8_t> &p_data) {
	CFA835::CommandCode command = CFA835::CommandCode::WRITE_USER_FLASH_AREA;
	send_message(command, p_data);
	std::vector<uint8_t> result = receive_message(command, 0);
	return result.size() == 0;
}

std::vector<uint8_t> CFA835::read_user_flash_area(uint8_t p_num_bytes) {
	std::vector<uint8_t> data{ p_num_bytes };

	CFA835::CommandCode command = CFA835::CommandCode::READ_USER_FLASH_AREA;
	send_message(command, data);
	std::vector<uint8_t> result = receive_message(command, p_num_bytes);
	return result;
}

bool CFA835::store_current_state_as_boot_state() {
	CFA835::CommandCode command = CFA835::CommandCode::STORE_CURRENT_STATE_AS_BOOT_STATE;
	send_message(command, std::vector<uint8_t>());
	std::vector<uint8_t> result = receive_message(command, 0);
	return result.size() == 0;
}

bool CFA835::reload_boot_settings() {
	CFA835::CommandCode command = CFA835::CommandCode::RESTART;
	std::vector<uint8_t> data = { 8, 18, 99 };
	send_message(command, data);
	std::vector<uint8_t> result = receive_message(command, 0);
	return result.size() == 0;
}

bool CFA835::restart_host() {
	CFA835::CommandCode command = CFA835::CommandCode::RESTART;
	std::vector<uint8_t> data = { 12, 28, 97 };
	send_message(command, data);
	std::vector<uint8_t> result = receive_message(command, 0);
	return result.size() == 0;
}

// WR-PWR-Y25 ATX Power Switch Cable Required
bool CFA835::power_off_host() {
	CFA835::CommandCode command = CFA835::CommandCode::RESTART;
	std::vector<uint8_t> data = { 3, 11, 95 };
	send_message(command, data);
	std::vector<uint8_t> result = receive_message(command, 0);
	return result.size() == 0;
}

bool CFA835::cfa835_restart() {
	CFA835::CommandCode command = CFA835::CommandCode::RESTART;
	std::vector<uint8_t> data = { 8, 25, 48 };
	send_message(command, data);
	std::vector<uint8_t> result = receive_message(command, 0);
	return result.size() == 0;
}

bool CFA835::cfa835_restore_default_settings() {
	CFA835::CommandCode command = CFA835::CommandCode::RESTART;
	std::vector<uint8_t> data = { 10, 8, 98 };
	send_message(command, data);
	std::vector<uint8_t> result = receive_message(command, 0);
	return result.size() == 0;
}

bool CFA835::clear_display() {
	CFA835::CommandCode command = CFA835::CommandCode::CLEAR_DISPLAY;
	send_message(command, std::vector<uint8_t>());
	std::vector<uint8_t> result = receive_message(command, 0);
	return result.size() == 0;
}

std::vector<uint8_t> CFA835::read_special_character_bitmaps(uint8_t p_index) {
	CFA835::CommandCode command = CFA835::CommandCode::SPECIAL_CHARACTER_BITMAPS;
	assert(0 <= p_index && p_index < 7);

	send_message(command, std::vector<uint8_t>{ p_index });
	uint8_t response_size = 9;
	std::vector<uint8_t> result = receive_message(command, response_size);
	return result;
}

bool CFA835::write_special_character_bitmaps(uint8_t p_index, const std::vector<uint8_t> &p_bitmap) {
	CFA835::CommandCode command = CFA835::CommandCode::SPECIAL_CHARACTER_BITMAPS;

	uint8_t data_size = 8;
	assert(p_bitmap.size() == data_size);
	assert(0 <= p_index && p_index < 7);
	std::vector<uint8_t> data{ p_index };
	data.insert(data.end(), p_bitmap.begin(), p_bitmap.end());
	send_message(command, data);

	std::vector<uint8_t> result = receive_message(command, 0);
	return result.size() == 0;
}

Vector2i CFA835::show_cursor() {
	CFA835::CommandCode command = CFA835::CommandCode::DISPLAY_CURSOR_POSITION;
	send_message(command, std::vector<uint8_t>{});
	int result_size = 2;
	std::vector<uint8_t> result = receive_message(command, result_size);
	return Vector2i(result[0], result[1]);
}

void CFA835::set_cursor_position(Vector2i p_position) {
	assert(0 <= p_position.x && p_position.x <= 19);
	assert(0 <= p_position.y && p_position.y <= 3);

	CFA835::CommandCode command = CFA835::CommandCode::DISPLAY_CURSOR_POSITION;
	send_message(command, std::vector<uint8_t>{ static_cast<uint8_t>(p_position.x), static_cast<uint8_t>(p_position.y) });
	int result_size = 0;
	receive_message(command, result_size);
}

CFA835::CursorStyle CFA835::get_cursor_style() {
	CFA835::CommandCode command = CFA835::CommandCode::CURSOR_STYLE;
	send_message(command, std::vector<uint8_t>());

	int result_size = 1;
	std::vector<uint8_t> result = receive_message(command, result_size);
	return static_cast<CFA835::CursorStyle>(result[0]);
}

void CFA835::set_cursor_style(CursorStyle p_style) {
	CFA835::CommandCode command = CFA835::CommandCode::CURSOR_STYLE;
	send_message(command, std::vector<uint8_t>{ static_cast<uint8_t>(p_style) });

	int result_size = 0;
	receive_message(command, result_size);
}

uint8_t CFA835::get_constrat() {
	CFA835::CommandCode command = CFA835::CommandCode::CONTRAST;
	send_message(command, std::vector<uint8_t>());

	int result_size = 1;
	std::vector<uint8_t> result = receive_message(command, result_size);
	uint8_t constrat_setting = result[0];
	return constrat_setting;
}

void CFA835::set_constrat(uint8_t p_constrat_setting) {
	CFA835::CommandCode command = CFA835::CommandCode::CONTRAST;
	send_message(command, std::vector<uint8_t>{ p_constrat_setting });

	int result_size = 0;
	receive_message(command, result_size);
}

uint8_t CFA835::get_display_brightness() {
	CFA835::CommandCode command = CFA835::CommandCode::DISPLAY_AND_KEYPAD_BACKLIGHTS;
	send_message(command, std::vector<uint8_t>());

	int result_size = 2;
	std::vector<uint8_t> result = receive_message(command, result_size);
	return result[0];
}

void CFA835::set_display_brightness(uint8_t p_display_brightness) {
	assert(0 <= p_display_brightness && p_display_brightness <= 100);

	CFA835::CommandCode command = CFA835::CommandCode::DISPLAY_AND_KEYPAD_BACKLIGHTS;
	send_message(command, std::vector<uint8_t>{ p_display_brightness, get_keypad_brightness() });

	int result_size = 0;
	std::vector<uint8_t> result = receive_message(command, result_size);
}

uint8_t CFA835::get_keypad_brightness() {
	CFA835::CommandCode command = CFA835::CommandCode::DISPLAY_AND_KEYPAD_BACKLIGHTS;
	send_message(command, std::vector<uint8_t>());

	int result_size = 2;
	std::vector<uint8_t> result = receive_message(command, result_size);
	return result[1];
}

void CFA835::set_keypad_brightness(uint8_t p_keypad_brightness) {
	assert(0 <= p_keypad_brightness && p_keypad_brightness <= 100);

	CFA835::CommandCode command = CFA835::CommandCode::DISPLAY_AND_KEYPAD_BACKLIGHTS;
	send_message(command, std::vector<uint8_t>{ get_display_brightness(), p_keypad_brightness });

	int result_size = 0;
	std::vector<uint8_t> result = receive_message(command, result_size);
}

CFA835::KeypadEvent CFA835::get_key_events() {
	CFA835::CommandCode command = CFA835::CommandCode::KEYPAD_REPORTING;
	send_message(command, std::vector<uint8_t>());

	int result_size = 2;
	std::vector<uint8_t> result = receive_message(command, result_size);
	return CFA835::KeypadEvent(static_cast<KeyActivity>(result[0]), static_cast<KeyActivity>(result[1]));
}

void CFA835::set_key_events(KeyCode p_keypad_press_mask, KeyCode p_keypad_release_mask) {
	CFA835::CommandCode command = CFA835::CommandCode::KEYPAD_REPORTING;
	send_message(command, std::vector<uint8_t>{ static_cast<uint8_t>(p_keypad_press_mask), static_cast<uint8_t>(p_keypad_release_mask) });

	int result_size = 0;
	receive_message(command, result_size);
}

std::vector<uint8_t> CFA835::keypad_reporting(const std::vector<uint8_t> &p_data) {
	assert(p_data.size() == 0 || p_data.size() == 2);

	CFA835::CommandCode command = CFA835::CommandCode::KEYPAD_REPORTING;
	send_message(command, p_data);

	int result_size = p_data.size() == 0 ? 2 : 0;
	std::vector<uint8_t> result = receive_message(command, result_size);
	return result;
}

std::vector<uint8_t> CFA835::read_keypad_or_polled_mode() {
	CFA835::CommandCode command = CFA835::CommandCode::READ_KEYPAD_OR_POLLED_MODE;
	send_message(command, std::vector<uint8_t>());

	int result_size = 3;
	std::vector<uint8_t> result = receive_message(command, result_size);
	assert(result.size() == 3);
	return result;
}

// std::vector<uint8_t> CFA835::atx_functionality(const std::vector<uint8_t> &p_data) {
// 	assert(p_data.size() == 0 || (p_data.size() == 2 || p_data.size() == 1));

// 	CFA835::CommandCode command = CFA835::CommandCode::ATX_FUNCTIONALITY;
// 	send_message(command, p_data);

// 	int result_size = p_data.size() == 0 ? 2 : 0;
// 	std::vector<uint8_t> result = receive_message(command, result_size);
// 	return result;
// }

// std::vector<uint8_t> CFA835::watchdog(const std::vector<uint8_t> &p_data) {
// 	assert(p_data.size() == 0 || p_data.size() == 1);

// 	CFA835::CommandCode command = CFA835::CommandCode::WATCHDOG;
// 	send_message(command, p_data);

// 	int result_size = p_data.size() == 0 ? 1 : 0;
// 	std::vector<uint8_t> result = receive_message(command, result_size);
// 	return result;
// }

void CFA835::write_text_to_the_display(uint8_t p_column_position, uint8_t p_row_position, const std::string &p_text) {
	assert(0 <= p_column_position && p_column_position <= 19);
	assert(0 <= p_row_position && p_row_position <= 3);
	assert(2 <= p_text.size() || p_text.size() <= 21);

	CFA835::CommandCode command = CFA835::CommandCode::WRITE_TEXT_TO_THE_DISPLAY;
	std::vector<uint8_t> data{ p_column_position, p_row_position };
	data.insert(data.end(), p_text.begin(), p_text.end());
	send_message(command, data);

	int result_size = 0;
	receive_message(command, result_size);
}

std::string CFA835::read_text_from_the_display(uint8_t p_column_position, uint8_t p_row_position, uint8_t p_length) {
	assert(0 <= p_column_position && p_column_position <= 19);
	assert(0 <= p_row_position && p_row_position <= 3);
	assert(1 <= p_length && p_length <= 20);

	CFA835::CommandCode command = CFA835::CommandCode::READ_TEXT_FROM_THE_DISPLAY;
	std::vector<uint8_t> data{ p_column_position, p_row_position, p_length };

	send_message(command, data);

	int result_size = p_length;
	std::vector<uint8_t> result = receive_message(command, result_size);
	assert(result.size() == p_length);
	return std::string{ result.begin(), result.end() };
}

// CFA835::SerialInterfaceOption CFA835::read_serial_interface_option_flags() {
// 	CFA835::CommandCode command = CFA835::CommandCode::INTERFACE_OPTIONS;
// 	uint8_t interface = 0; // serial

// 	send_message(command, std::vector<uint8_t>(interface));

// 	int result_size = 3;
// 	std::vector<uint8_t> result = receive_message(command, result_size);
// 	assert(result[0] = interface);
// 	CFA835::InterfaceOption option_flags = static_cast<CFA835::InterfaceOption>(result[1]);
// 	CFA835::BaudRate baud_rate = static_cast<CFA835::BaudRate>(result[2]);
// 	return SerialInterfaceOption(option_flags, baud_rate);
// }

// CFA835::InterfaceOption CFA835::read_usb_interface_options_flags() {
// 	CFA835::CommandCode command = CFA835::CommandCode::INTERFACE_OPTIONS;
// 	uint8_t interface = 1; // USB

// 	send_message(command, std::vector<uint8_t>(interface));

// 	int result_size = 2;
// 	std::vector<uint8_t> result = receive_message(command, result_size);
// 	assert(result[0] = interface);
// 	CFA835::InterfaceOption option_flags = static_cast<CFA835::InterfaceOption>(result[1]);
// 	return option_flags;
// }

// CFA835::SPIInterfaceOption CFA835::read_spi_interface_options_flags() {
// 	CFA835::CommandCode command = CFA835::CommandCode::INTERFACE_OPTIONS;
// 	uint8_t interface = 2; // SPI

// 	send_message(command, std::vector<uint8_t>(interface));

// 	int result_size = 4;
// 	std::vector<uint8_t> result = receive_message(command, result_size);
// 	assert(result[0] = interface);
// 	CFA835::InterfaceOption option_flags = static_cast<CFA835::InterfaceOption>(result[1]);
// 	SpiCpol cpol = static_cast<CFA835::SpiCpol>(result[2] & 0b00000001); // Bit 0
// 	SpiCpha cpha = static_cast<CFA835::SpiCpha>(result[2] & 0b00000010); // Bit 1
// 	BitFirst bit_first = static_cast<CFA835::BitFirst>(result[2] & 0b00000100); // Bit 2
// 	uint8_t reserved = result[2] >> 3; // Bits 3-7

// 	return CFA835::SPIInterfaceOption(option_flags, cpol, cpha, bit_first, reserved);
// }

// CFA835::S2CInterfaceOption CFA835::read_i2c_interface_options_flags() {
// 	CFA835::CommandCode command = CFA835::CommandCode::INTERFACE_OPTIONS;
// 	uint8_t interface = 3; // I2C

// 	send_message(command, std::vector<uint8_t>(interface));

// 	int result_size = 4;
// 	std::vector<uint8_t> result = receive_message(command, result_size);
// 	assert(result[0] = interface);
// 	CFA835::InterfaceOption option_flags = static_cast<CFA835::InterfaceOption>(result[1]);
// 	uint8_t i2c_address = result[2];
// 	CFA835::I2CBusSpeed i2c_bus_speed = static_cast<CFA835::I2CBusSpeed>(result[3]);

// 	return CFA835::S2CInterfaceOption(option_flags, i2c_address, i2c_bus_speed);
// }

// void CFA835::write_serial_interface_option_flags(CFA835::SerialInterfaceOption p_options) {
// 	CFA835::CommandCode command = CFA835::CommandCode::INTERFACE_OPTIONS;
// 	uint8_t interface = 0; // Serial

// 	uint8_t option_flags = static_cast<uint8_t>(p_options.option_flags);
// 	uint8_t baud_rate = static_cast<uint8_t>(p_options.baud_rate);
// 	std::vector<uint8_t> data{ interface, option_flags, baud_rate };
// 	send_message(command, data);
// 	int result_size = 0;
// 	std::vector<uint8_t> result = receive_message(command, result_size);
// }

// void CFA835::write_usb_interface_options_flags(CFA835::InterfaceOption p_options) {
// 	CFA835::CommandCode command = CFA835::CommandCode::INTERFACE_OPTIONS;
// 	uint8_t interface = 1; // USB

// 	uint8_t option_flags = static_cast<uint8_t>(p_options);
// 	std::vector<uint8_t> data{ interface, option_flags };
// 	send_message(command, data);
// 	int result_size = 0;
// 	std::vector<uint8_t> result = receive_message(command, result_size);
// }

// void CFA835::write_spi_interface_options_flags(CFA835::SPIInterfaceOption p_options) {
// 	CFA835::CommandCode command = CFA835::CommandCode::INTERFACE_OPTIONS;
// 	uint8_t interface = 2; // SPI

// 	uint8_t option_flags = static_cast<uint8_t>(p_options.option_flags);
// 	uint8_t cpol = static_cast<uint8_t>(p_options.cpol);
// 	uint8_t cpha = static_cast<uint8_t>(p_options.cpha);
// 	uint8_t bit_first = static_cast<uint8_t>(p_options.bit_first);
// 	uint8_t reserved = static_cast<uint8_t>(p_options.reserved);

// 	uint8_t spi_mode_settings =
// 			(cpol << 0) | // Bit 0: cpol
// 			(cpha << 1) | // Bit 1: cpha
// 			(bit_first << 2) | // Bit 2: bit_first
// 			(reserved << 3); // Bits 3-7: reserved

// 	std::vector<uint8_t> data{ interface, option_flags, spi_mode_settings, reserved };
// 	send_message(command, data);
// 	int result_size = 0;
// 	std::vector<uint8_t> result = receive_message(command, result_size);
// }

// void CFA835::write_i2c_interface_options_flags(CFA835::S2CInterfaceOption p_options) {
// 	CFA835::CommandCode command = CFA835::CommandCode::INTERFACE_OPTIONS;
// 	uint8_t interface = 3; // I2C

// 	uint8_t option_flags = static_cast<uint8_t>(p_options.option_flags);
// 	uint8_t i2c_address = static_cast<uint8_t>(p_options.i2c_address);
// 	uint8_t bus_speed = static_cast<uint8_t>(p_options.speed);

// 	std::vector<uint8_t> data{ interface, option_flags, i2c_address, bus_speed };
// 	send_message(command, data);
// 	int result_size = 0;
// 	std::vector<uint8_t> result = receive_message(command, result_size);
// }

// CFA835::GPIOResult CFA835::read_gpio(CFA835::GPIOIndex p_index) {
// 	CFA835::CommandCode command = CFA835::CommandCode::GPIO_PIN_CONFIGURATION;

// 	std::vector<uint8_t> data{ static_cast<uint8_t>(p_index) };
// 	send_message(command, data);
// 	int result_size = 4;
// 	std::vector<uint8_t> response = receive_message(command, result_size);
// 	CFA835::GPIOIndex index = static_cast<CFA835::GPIOIndex>(response[0]);
// 	bool pin_state_s = response[1] & 0b00000001;
// 	bool pin_state_f = (response[1] & 0b00000010) >> 1;
// 	bool pin_state_r = (response[1] & 0b00000100) >> 2;
// 	uint8_t pin_pwm_output_value = response[2];
// 	uint8_t pin_function_select = response[3] & 0b00001111;
// 	uint8_t pin_drive_mode = (response[3] & 0b11110000) >> 4;

// 	CFA835::GPIOResult result(index, pin_state_s, pin_state_f, pin_state_r, pin_pwm_output_value, pin_function_select, pin_drive_mode);
// 	return result;
// }

// CFA835::ADCResult CFA835::read_adc(CFA835::ADCIndex p_index) {
// 	CFA835::CommandCode command = CFA835::CommandCode::GPIO_PIN_CONFIGURATION;

// 	std::vector<uint8_t> data{ static_cast<uint8_t>(p_index) };
// 	send_message(command, data);
// 	int result_size = 7;
// 	std::vector<uint8_t> response = receive_message(command, result_size);
// 	CFA835::ADCIndex index = static_cast<CFA835::ADCIndex>(data[0]);
// 	uint16_t average = (data[2] << 8) | data[1];
// 	uint16_t min_value = (data[4] << 8) | data[3];
// 	uint16_t max_value = (data[6] << 8) | data[5];

// 	return CFA835::ADCResult(index, average, min_value, max_value);
// }

void CFA835::configure_gpio(GPIOIndex p_gpio, uint8_t p_pin_output_state) {
	assert(0 <= p_pin_output_state && p_pin_output_state <= 100);

	CFA835::CommandCode command = CFA835::CommandCode::GPIO_PIN_CONFIGURATION;

	std::vector<uint8_t> data{ static_cast<uint8_t>(p_gpio), p_pin_output_state };
	send_message(command, data);
	int result_size = 0;
	std::vector<uint8_t> response = receive_message(command, result_size);
}

void CFA835::load_font_from_micro_sd_card(uint8_t p_font_slot, uint8_t p_option_flags, std::string p_filename) {
	assert(0 <= p_font_slot && p_font_slot <= 3);
	assert(3 <= p_filename.size() && p_filename.size() <= 123);

	CFA835::CommandCode command = CFA835::CommandCode::CUSTOM_FONTS_COMMAND_GROUP;
	uint8_t slash = '\\';

	std::vector<uint8_t> data{ 0 /*load custom font*/, p_font_slot, p_option_flags, slash };
	data.insert(data.end(), p_filename.begin(), p_filename.end());

	send_message(command, data);
	int result_size = 1;
	std::vector<uint8_t> response = receive_message(command, result_size);
	assert(response[0] == 0 /*load custom font*/);
}

void CFA835::print_custom_font_to_display(uint8_t p_font_slot, Vector2i p_position, std::string p_text) {
	static uint8_t MAX_TEXT_SIZE = 124 - 4;
	assert(0 <= p_font_slot && p_font_slot <= 3);
	assert(p_text.size() < MAX_TEXT_SIZE);

	CFA835::CommandCode command = CFA835::CommandCode::CUSTOM_FONTS_COMMAND_GROUP;

	std::vector<uint8_t> data{ 1 /*print custom font*/, p_font_slot, 1 /* place character base on pixel*/,
		static_cast<uint8_t>(p_position.x), static_cast<uint8_t>(p_position.y) };
	data.insert(data.end(), p_text.begin(), p_text.end());
	data.push_back('\0');
	send_message(command, data);
	int result_size = 2;
	std::vector<uint8_t> response = receive_message(command, result_size);
	assert(response[0] == 1);
}

void CFA835::open_micro_sd_file(uint8_t p_option, std::string p_filename) {
	if (is_file_opened) {
		close_current_opened_micro_sd_file();
	}
	static uint8_t MAX_FILENAME_SIZE = 124 - 2;
	assert(1 <= p_option && p_option <= 3);
	assert(p_filename.size() < MAX_FILENAME_SIZE);

	CFA835::CommandCode command = CFA835::CommandCode::MICROSD_FILE_OPERATIONS_COMMAND_GROUP;
	uint8_t slash = '\\';

	std::vector<uint8_t> data{ 0 /*open file*/, p_option, slash };
	data.insert(data.end(), p_filename.begin(), p_filename.end());
	send_message(command, data);
	int result_size = 5;
	std::vector<uint8_t> response = receive_message(command, result_size);
	assert(response[0] == 0 /*OPEN FILE*/);
	is_file_opened = true;
}

void CFA835::close_current_opened_micro_sd_file() {
	CFA835::CommandCode command = CFA835::CommandCode::MICROSD_FILE_OPERATIONS_COMMAND_GROUP;

	std::vector<uint8_t> data{ 0 /*open file*/, 0 /*close currently opened file*/ };
	send_message(command, data);
	int result_size = 5;
	std::vector<uint8_t> response = receive_message(command, result_size);
	assert(response[0] == 0 /*OPEN FILE*/);
	is_file_opened = false;
}

void CFA835::micro_sd_file_position_seek(size_t p_location) {
	if (!is_file_opened) {
		throw std::runtime_error("File is not open");
	}

	CFA835::CommandCode command = CFA835::CommandCode::MICROSD_FILE_OPERATIONS_COMMAND_GROUP;
	std::vector<uint8_t> data(5);
	data[0] = 1; // Position seek
	data[1] = static_cast<uint8_t>(p_location & 0xFF); // Extract the least significant byte
	data[2] = static_cast<uint8_t>((p_location >> 8) & 0xFF); // Extract the next byte
	data[3] = static_cast<uint8_t>((p_location >> 16) & 0xFF); // Extract the next byte
	data[4] = static_cast<uint8_t>((p_location >> 24) & 0xFF); // Extract the most significant byte

	send_message(command, data);
	int result_size = 1;
	std::vector<uint8_t> response = receive_message(command, result_size);
	assert(response[0] == 1 /*Position seek*/);
}

std::vector<uint8_t> CFA835::micro_sd_file_read_data(uint8_t p_num_bytes) {
	assert(1 <= p_num_bytes && p_num_bytes <= 123);

	if (!is_file_opened) {
		throw std::runtime_error("File is not open");
	}

	CFA835::CommandCode command = CFA835::CommandCode::MICROSD_FILE_OPERATIONS_COMMAND_GROUP;
	std::vector<uint8_t> data{ 2, p_num_bytes };

	send_message(command, data);
	int result_size = p_num_bytes + 1;
	std::vector<uint8_t> response = receive_message(command, result_size);
	assert(response[0] == 2 /*Read file data*/);

	return std::vector<uint8_t>(response.begin() + 1, response.end());
}

void CFA835::micro_sd_file_write_data(std::vector<uint8_t> p_data) {
	assert(1 <= p_data.size() && p_data.size() <= 124);

	if (!is_file_opened) {
		throw std::runtime_error("File is not open");
	}

	CFA835::CommandCode command = CFA835::CommandCode::MICROSD_FILE_OPERATIONS_COMMAND_GROUP;
	std::vector<uint8_t> data{ 3 /*Write file data*/ };
	data.insert(data.end(), p_data.begin(), p_data.end());

	send_message(command, data);
	int result_size = 1;
	std::vector<uint8_t> response = receive_message(command, result_size);
	assert(response[0] == 3 /*Write file data*/);
}

void CFA835::micro_sd_file_delete(std::string p_filename) {
	assert(1 <= p_filename.size() && p_filename.size() <= 124);

	CFA835::CommandCode command = CFA835::CommandCode::MICROSD_FILE_OPERATIONS_COMMAND_GROUP;
	uint8_t slash = '\\';
	std::vector<uint8_t> data{ 4 /*Delete a file*/, slash };
	data.insert(data.end(), p_filename.begin(), p_filename.end());

	send_message(command, data);
	int result_size = 1;
	std::vector<uint8_t> response = receive_message(command, result_size);
	assert(response[0] == 4 /*Delete a file*/);
}

void CFA835::set_manual_flush_graphic_buffer(bool p_enable) {
	CFA835::CommandCode command = CFA835::CommandCode::DISPLAY_GRAPHIC_OPTIONS_COMMAND_GROUP;
	uint8_t graphic_option = 0;
	uint8_t manual_flush = p_enable << 0;
	std::vector<uint8_t> data{ graphic_option, manual_flush };

	send_message(command, data);
	int result_size = 1;
	std::vector<uint8_t> response = receive_message(command, result_size);
	assert(response[0] == graphic_option);
}

void CFA835::set_gamma_correction(bool p_enable) {
	CFA835::CommandCode command = CFA835::CommandCode::DISPLAY_GRAPHIC_OPTIONS_COMMAND_GROUP;
	uint8_t graphic_option = 0;
	uint8_t gamma_correction = p_enable << 1;
	std::vector<uint8_t> data{ graphic_option, gamma_correction };

	send_message(command, data);
	int result_size = 1;
	std::vector<uint8_t> response = receive_message(command, result_size);
	assert(response[0] == graphic_option);
}

void CFA835::flush_buffer() {
	CFA835::CommandCode command = CFA835::CommandCode::DISPLAY_GRAPHIC_OPTIONS_COMMAND_GROUP;
	uint8_t flush = 1;

	std::vector<uint8_t> data{ flush };

	send_message(command, data);
	int result_size = 1;
	std::vector<uint8_t> response = receive_message(command, result_size);
	assert(response[0] == flush);
}
void CFA835::send_bmp_to_display_from_host(const std::vector<uint8_t> &p_data, const Vector2i &p_position, const Vector2i &p_size) {
	CFA835::CommandCode command = CFA835::CommandCode::DISPLAY_GRAPHIC_OPTIONS_COMMAND_GROUP;
	uint8_t send_image_data = 2;
	uint8_t option_flags = 0; // transparent.
	uint8_t position_x = static_cast<uint8_t>(p_position.x);
	uint8_t position_y = static_cast<uint8_t>(p_position.y);

	int32_t width = p_size.x;
	int32_t height = p_size.y;

	std::vector<uint8_t> bmp = p_data;

	std::vector<uint8_t> data{ send_image_data, option_flags, position_x, position_y, static_cast<uint8_t>(width), static_cast<uint8_t>(height) };

	send_message(command, data);
	send_data(bmp);
	int result_size = 1;
	std::vector<uint8_t> response = receive_message(command, result_size);
	assert(response[0] == send_image_data);
}

void CFA835::send_bmp_to_display_from_host(const std::string &p_bmp_path, const Vector2i &p_position) {
	int32_t width = 0;
	int32_t height = 0;

	std::vector<uint8_t> bmp = get_bitmap_data(p_bmp_path, width, height);

	send_bmp_to_display_from_host(bmp, p_position, Vector2i(width, height));
}

void CFA835::set_pixel_data(Vector2i p_position, uint8_t p_shaded) {
	uint8_t position_x = static_cast<uint8_t>(p_position.x);
	uint8_t position_y = static_cast<uint8_t>(p_position.y);

	assert(0 <= position_x && position_x < SCREEN_WIDTH);
	assert(0 <= position_y && position_y < SCREEN_HEIGHT);

	CFA835::CommandCode command = CFA835::CommandCode::DISPLAY_GRAPHIC_OPTIONS_COMMAND_GROUP;
	std::vector<uint8_t> data{ 5 /*Pixel Data*/, position_x, position_y, p_shaded };

	send_message(command, data);
	int result_size = 1;
	std::vector<uint8_t> response = receive_message(command, result_size);
	assert(response[0] == 5 /*Pixel Data*/);
}

uint8_t CFA835::get_pixel_data(Vector2i p_position) {
	uint8_t position_x = static_cast<uint8_t>(p_position.x);
	uint8_t position_y = static_cast<uint8_t>(p_position.y);

	assert(0 <= position_x && position_x < SCREEN_WIDTH);
	assert(0 <= position_y && position_y < SCREEN_HEIGHT);

	CFA835::CommandCode command = CFA835::CommandCode::DISPLAY_GRAPHIC_OPTIONS_COMMAND_GROUP;
	std::vector<uint8_t> data{ 5 /*Pixel Data*/, position_x, position_y };

	send_message(command, data);
	int result_size = 2;
	std::vector<uint8_t> response = receive_message(command, result_size);
	assert(response[0] == 5 /*Pixel Data*/);

	return response[1];
}

void CFA835::draw_a_line(Vector2i p_start_position, Vector2i p_end_position, uint8_t p_line_shade) {
	uint8_t start_x = static_cast<uint8_t>(p_start_position.x);
	uint8_t start_y = static_cast<uint8_t>(p_start_position.y);

	uint8_t end_x = static_cast<uint8_t>(p_end_position.x);
	uint8_t end_y = static_cast<uint8_t>(p_end_position.y);

	assert(0 <= start_x && start_x < SCREEN_WIDTH);
	assert(0 <= start_y && start_y < SCREEN_HEIGHT);

	assert(0 <= end_x && end_x < SCREEN_WIDTH);
	assert(0 <= end_y && end_y < SCREEN_HEIGHT);
	assert(0 <= p_line_shade && p_line_shade <= 255);

	CFA835::CommandCode command = CFA835::CommandCode::DISPLAY_GRAPHIC_OPTIONS_COMMAND_GROUP;
	std::vector<uint8_t> data{ 6 /*Draw a line*/, start_x, start_y, end_x, end_y, p_line_shade };

	send_message(command, data);
	int result_size = 1;
	std::vector<uint8_t> response = receive_message(command, result_size);
	assert(response[0] == 6 /*Draw a line*/);
}

void CFA835::draw_a_rectangle(Vector2i p_start_position, Vector2i p_size, uint8_t p_line_shade, uint8_t p_fill_shade) {
	uint8_t start_x = static_cast<uint8_t>(p_start_position.x);
	uint8_t start_y = static_cast<uint8_t>(p_start_position.y);

	uint8_t width = static_cast<uint8_t>(p_size.x);
	uint8_t height = static_cast<uint8_t>(p_size.y);

	assert(0 <= start_x && start_x < SCREEN_WIDTH);
	assert(0 <= start_y && start_y < SCREEN_HEIGHT);

	assert(0 <= width && width < SCREEN_WIDTH);
	assert(0 <= height && height < SCREEN_HEIGHT);
	assert(0 <= p_line_shade && p_line_shade <= 255);
	assert(0 <= p_fill_shade && p_fill_shade <= 255);

	CFA835::CommandCode command = CFA835::CommandCode::DISPLAY_GRAPHIC_OPTIONS_COMMAND_GROUP;
	std::vector<uint8_t> data{ 7 /*Draw a rectangle*/, start_x, start_y, width, height, p_line_shade, p_fill_shade };

	send_message(command, data);
	int result_size = 1;
	std::vector<uint8_t> response = receive_message(command, result_size);
	assert(response[0] == 7 /*Draw a rectangle*/);
}

void CFA835::draw_a_circle(Vector2i p_position, uint8_t p_radius, uint8_t p_line_shade, uint8_t p_fill_shade) {
	uint8_t position_x = static_cast<uint8_t>(p_position.x);
	uint8_t position_y = static_cast<uint8_t>(p_position.y);

	assert(0 <= position_x && position_x < SCREEN_WIDTH);
	assert(0 <= position_y && position_y < SCREEN_HEIGHT);
	auto is_circle_within_screen = [this](int x, int y, int radius) {
		return (x - radius >= 0) &&
				(x + radius < SCREEN_WIDTH) &&
				(y - radius >= 0) &&
				(y + radius < SCREEN_HEIGHT);
	};

	assert(is_circle_within_screen(position_x, position_y, p_radius));
	// Avoid drawing outside the screen.
	assert(0 <= position_x && position_x < SCREEN_WIDTH);
	assert(0 <= position_y && position_y < SCREEN_HEIGHT);

	assert(0 <= p_line_shade && p_line_shade <= 255);
	assert(0 <= p_fill_shade && p_fill_shade <= 255);

	CFA835::CommandCode command = CFA835::CommandCode::DISPLAY_GRAPHIC_OPTIONS_COMMAND_GROUP;
	std::vector<uint8_t> data{ 8 /*Draw a circle*/, position_x, position_y, p_radius, p_line_shade, p_fill_shade };

	send_message(command, data);
	int result_size = 1;
	std::vector<uint8_t> response = receive_message(command, result_size);
	assert(response[0] == 8 /*Draw a circle*/);
}

void CFA835::upload_file(std::string p_path) {
	static int8_t truncates_existing_file = 2;
	std::filesystem::path path(p_path);
	std::string filename = path.filename().string();

	open_micro_sd_file(truncates_existing_file, filename);

	std::ifstream file(p_path, std::ios::binary);

	if (!file.is_open()) {
		std::cerr << "Error opening file: " << p_path << std::endl;
		return;
	}

	std::vector<uint8_t> buffer(120);

	// Read and store the contents of the file, 120 bytes at a time
	while (file.read(reinterpret_cast<char *>(buffer.data()), buffer.size())) {
		std::streamsize bytesRead = file.gcount();
		if (bytesRead > 0) {
			buffer.resize(bytesRead);
			micro_sd_file_write_data(buffer);
		}
	}

	file.close();

	close_current_opened_micro_sd_file();
}
