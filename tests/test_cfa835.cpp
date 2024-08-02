#include "catch.hpp"

#include "hardwares/crystalfontz/cfa835.h"
#include "managers/device_manager.h"
#include "math/vector2i.h"

#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include <thread>

// TEST_CASE("Test ping", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	std::string msg = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis elementum dolor vitae tristique donec. ";

// 	std::string response_msg = device->ping(msg);

// 	REQUIRE(response_msg == msg);
// }

// TEST_CASE("Test get_module_information", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	SECTION("Test get_hardware_and_firmware_info") {
// 		std::string hardware_and_firmware_info = device->get_hardware_and_firmware_info();
// 		REQUIRE(hardware_and_firmware_info != "");
// 		REQUIRE(hardware_and_firmware_info.rfind(CFA835::DEVICE_IDENTIFIER, 0) == 0);
// 	}

// 	SECTION("Test get_serial_number") {
// 		std::string serial = device->get_serial_number();

// 		REQUIRE(serial != "");
// 	}
// }

// TEST_CASE("Test write_user_flash_area", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	std::string msg = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis elementum dolor vitae tristique donec. ";
// 	std::vector<uint8_t> data(msg.begin(), msg.end());

// 	bool write_user_flash_area_sucess = device->write_user_flash_area(data);

// 	REQUIRE(write_user_flash_area_sucess);
// }

// TEST_CASE("Test read_user_flash_area", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	std::string msg = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis elementum dolor vitae tristique donec. ";
// 	std::vector<uint8_t> data(msg.begin(), msg.end());

// 	std::vector<uint8_t> result = device->read_user_flash_area(data.size());

// 	REQUIRE(result == data);
// }

// TEST_CASE("Test store_current_state_as_boot_state", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);

// 	bool state_stored_sucessfully = device->store_current_state_as_boot_state();

// 	REQUIRE(state_stored_sucessfully == true);
// }

// TEST_CASE("Test restart", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);

// 	SECTION("Test reload_boot_settings") {
// 		bool reload_boot_settings_sucessfully = device->reload_boot_settings();

// 		REQUIRE(reload_boot_settings_sucessfully == true);
// 	}

// 	// These test scenarios are provided for demonstration purposes and are not run by default.
// 	// This is because when the device restarts, it disconnects from the PC and reconnects,
// 	// causing the DeviceManager to repeatedly invoke initialization and resulting in an infinite loop.

// 	// SECTION("Test restart_host") {
// 	// 	bool restart_host_sucessfully = device->restart_host();

// 	// 	REQUIRE(restart_host_sucessfully == true);
// 	// }

// 	// SECTION("Test power_off_host") {
// 	// 	bool power_off_host_sucessfully = device->power_off_host();

// 	// 	REQUIRE(power_off_host_sucessfully == true);
// 	// }

// 	// SECTION("Test cfa835_restart") {
// 	// 	bool cfa835_restart_sucessfully = device->cfa835_restart();

// 	// 	REQUIRE(cfa835_restart_sucessfully == true);
// 	// }

// 	// SECTION("Test cfa835_restore_default_settings") {
// 	// 	bool cfa835_restore_default_settings = device->cfa835_restore_default_settings();

// 	// 	REQUIRE(cfa835_restore_default_settings == true);
// 	// }
// }

TEST_CASE("Test clear_display", "[CFA835]") {
	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);

	bool screen_is_cleared = device->clear_display();

	REQUIRE(screen_is_cleared == true);
}

// TEST_CASE("Test read_special_character_bitmaps", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	uint8_t index = 1;
// 	std::vector<uint8_t> data = device->read_special_character_bitmaps(index);

// 	REQUIRE(data[0] == index);
// }

// TEST_CASE("Test write_special_character_bitmaps", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	uint8_t index = 1;
// 	bool write_operation_sucessfully = device->write_special_character_bitmaps(index, std::vector<uint8_t>(8));

// 	REQUIRE(write_operation_sucessfully == true);
// }

// TEST_CASE("Test cursor_style", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	CFA835::CursorStyle style = CFA835::CursorStyle::UNDERSCORE;
// 	SECTION("Test set_cursor_style") {
// 		device->set_cursor_style(style);
// 	}
// 	SECTION("Test get_cursor_style") {
// 		CFA835::CursorStyle cursor_style = device->get_cursor_style();

// 		REQUIRE(cursor_style == style);
// 	}
// }

// TEST_CASE("Test display_cursor_position", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	SECTION("Test show_cursor") {
// 		Vector2i result = device->show_cursor();

// 		REQUIRE(result != Vector2i(20, 20));
// 	}
// 	SECTION("Test set_cursor_position") {
// 		int column_size = 19;
// 		int row_size = 3;
// 		for (uint8_t row = 0; row <= row_size; ++row) {
// 			for (uint8_t column = 0; column <= column_size; ++column) {
// 				device->set_cursor_position(Vector2i(column, row));
// 				// For debug purpose. Because it runs so fast and we cannot see anything.
// 				std::this_thread::sleep_for(std::chrono::milliseconds(300));
// 			}
// 		}
// 	}
// }

// TEST_CASE("Test contrast", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	SECTION("Test get_constrat") {
// 		int constrat_setting = device->get_constrat();
// 		REQUIRE(constrat_setting <= int(255));
// 	}
// 	SECTION("Test set_constrat") {
// 		int about_right = 200;
// 		device->set_constrat(about_right);
// 		int after_set_constrat = device->get_constrat();
// 		REQUIRE(after_set_constrat == about_right);
// 	}
// }

// TEST_CASE("Test 14 (0x0E): Display and Keypad Backlights", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	SECTION("Test get_display_brightness") {
// 		uint8_t current_display_brightness = device->get_display_brightness();
// 		REQUIRE(int(current_display_brightness) <= 100);
// 	}
// 	SECTION("Test get_keypad_brightness") {
// 		uint8_t current_keypad_brightness = device->get_keypad_brightness();
// 		REQUIRE(int(current_keypad_brightness) <= 100);
// 	}

// 	SECTION("Test set_display_brightness") {
// 		size_t max_brightness = 100;
// 		for (size_t display_brightness = 0; display_brightness <= max_brightness; ++display_brightness) {
// 			device->set_display_brightness(display_brightness);
// 			REQUIRE(int(display_brightness) == int(device->get_display_brightness()));
// 			// For debug purpose. Because it runs so fast and we cannot see anything.
// 			// std::this_thread::sleep_for(std::chrono::milliseconds(100));
// 		}
// 	}
// 	SECTION("Test set_keypad_brightness") {
// 		size_t max_brightness = 100;
// 		for (size_t keypad_brightness = 0; keypad_brightness <= max_brightness; ++keypad_brightness) {
// 			device->set_keypad_brightness(keypad_brightness);
// 			REQUIRE(int(keypad_brightness) == int(device->get_keypad_brightness()));
// 			// For debug purpose. Because it runs so fast and we cannot see anything.
// 			// std::this_thread::sleep_for(std::chrono::milliseconds(100));
// 		}
// 	}
// }

// TEST_CASE("Test 23 (0x17): Keypad Reporting", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	SECTION("Test get_key_events") {
// 		// while (true) {
// 		// 	CFA835::KeypadEvent key_event = device->get_key_events();
// 		// 	REQUIRE(static_cast<int>(key_event.key_press_mask) <= 63);
// 		// 	REQUIRE(static_cast<int>(key_event.key_release_mask) <= 63);
// 		// }
// 	}
// 	SECTION("Test set_key_events") {
// 		while (true) {
// 			KeyCode keypad_press_mask = KeyCode::CANCEL | KeyCode::ENTER;
// 			KeyCode keypad_release_mask = KeyCode::ENTER | KeyCode::CANCEL;

// 			device->set_key_events(keypad_press_mask, keypad_release_mask);
// 		}
// 	}
// }

// TEST_CASE("Test read_keypad_or_polled_mode", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	std::vector<uint8_t> result = device->read_keypad_or_polled_mode();
// 	REQUIRE(result.size() == 3);
// 	uint8_t bitmask_indicating_the_keys_currently_pressed = result[0];
// 	uint8_t bitmask_indicating_the_keys_pressed_since_the_last_poll = result[1];
// 	uint8_t bitmask_indicating_the_keys_released_since_the_last_poll = result[2];
// 	REQUIRE(bitmask_indicating_the_keys_currently_pressed == 0);
// 	REQUIRE(bitmask_indicating_the_keys_pressed_since_the_last_poll == 0);
// 	REQUIRE(bitmask_indicating_the_keys_released_since_the_last_poll == 0);
// }

// TEST_CASE("Test atx_functionality", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);

// 	SECTION("Test atx_functionality READ") {
// 		std::vector<uint8_t> result = device->atx_functionality(std::vector<uint8_t>());
// 		REQUIRE(result.size() == 2);
// 		// uint8_t bitmask_of_enabled_functions = result[0];
// 		// uint8_t length_of_power_on_and_off_pulses_in_1_per_32_second_increments = result[1];
// 	}
// 	SECTION("Test atx_functionality WRITE") {
// 		uint8_t enabled_functions = static_cast<uint8_t>(CFA835::AtxBitmasks::AUTO_POLARITY);
// 		uint8_t length_of_power_on_and_off_pulses_in_1_per_32_second_increments = 1;
// 		std::vector<uint8_t> data{ enabled_functions, length_of_power_on_and_off_pulses_in_1_per_32_second_increments };
// 		std::vector<uint8_t> result = device->atx_functionality(data);
// 		REQUIRE(result.size() == 0);
// 	}
// }

// TEST_CASE("Test watchdog", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);

// 	SECTION("Test watchdog READ") {
// 		std::vector<uint8_t> result = device->watchdog(std::vector<uint8_t>());
// 		REQUIRE(result.size() == 1);
// 		uint8_t watchdog_timeout_in_seconds = result[0];
// 		REQUIRE(watchdog_timeout_in_seconds != -1);
// 	}
// 	SECTION("Test watchdog WRITE") {
// 		uint8_t watchdog_is_disabled = 0;
// 		std::vector<uint8_t> data{ watchdog_is_disabled };
// 		std::vector<uint8_t> result = device->watchdog(data);
// 		REQUIRE(result.size() == 0);
// 	}
// }

// TEST_CASE("Test write_text_to_the_display", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);

// 	uint8_t column_position = 0;
// 	uint8_t row_position = 3;
// 	std::string msg = "Lorem ipsum dolor";
// 	device->write_text_to_the_display(column_position, row_position, msg);
// }

// TEST_CASE("Test read_text_from_the_display", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);

// 	uint8_t column_position = 0;
// 	uint8_t row_position = 3;
// 	std::string msg = "Lorem ipsum dolor";

// 	std::string result = device->read_text_from_the_display(column_position, row_position, msg.size());
// 	REQUIRE(result == msg);
// }

// TEST_CASE("Test Interface Options", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	// SECTION("Test read_serial_interface_option_flags") {
// 	// 	CFA835::SerialInterfaceOption result = device->read_serial_interface_option_flags();

// 	// 	REQUIRE(result.option_flags != CFA835::InterfaceOption::COUNT);
// 	// 	REQUIRE(result.baud_rate != CFA835::BaudRate::COUNT);
// 	// }

// 	// SECTION("Test read_usb_interface_options_flags") {
// 	// 	CFA835::InterfaceOption option_flags = device->read_usb_interface_options_flags();

// 	// 	REQUIRE(option_flags != CFA835::InterfaceOption::COUNT);
// 	// }

// 	// SECTION("Test read_spi_interface_options_flags") {
// 	// 	CFA835::SPIInterfaceOption interface_options = device->read_spi_interface_options_flags();

// 	// 	REQUIRE(interface_options.option_flags != CFA835::InterfaceOption::COUNT);
// 	// 	REQUIRE(interface_options.cpol != CFA835::SpiCpol::COUNT);
// 	// 	REQUIRE(interface_options.cpha != CFA835::SpiCpha::COUNT);
// 	// 	REQUIRE(interface_options.bit_first != CFA835::BitFirst::COUNT);
// 	// 	REQUIRE(interface_options.reserved != -1);
// 	// }

// 	// SECTION("Test read_i2c_interface_options_flags") {
// 	// 	CFA835::S2CInterfaceOption interface_options = device->read_i2c_interface_options_flags();

// 	// 	REQUIRE(interface_options.option_flags != CFA835::InterfaceOption::COUNT);
// 	// 	REQUIRE(interface_options.i2c_address != -1);
// 	// 	REQUIRE(interface_options.speed != CFA835::I2CBusSpeed::COUNT);
// 	// }

// 	// SECTION("Test write_serial_interface_option_flags") {
// 	// 	CFA835::InterfaceOption flags = CFA835::InterfaceOption::CFA835_WILL_SEND_ERRORS_FROM_COMMANDS_RECEIVED_ON_ANY_INTERFACE;
// 	// 	CFA835::SerialInterfaceOption options(flags, CFA835::BaudRate::_115200);
// 	// 	device->write_serial_interface_option_flags(options);
// 	// }

// 	// SECTION("Test write_usb_interface_options_flags") {
// 	// 	CFA835::InterfaceOption flags = CFA835::InterfaceOption::ENABLE_INTERFACE | CFA835::InterfaceOption::COMMAND_INTERPRETER_ENABLED | CFA835::InterfaceOption::CFA835_WILL_SEND_ERRORS_FROM_COMMANDS_RECEIVED_ON_ANY_INTERFACE;
// 	// 	device->write_usb_interface_options_flags(flags);
// 	// }

// 	// SECTION("Test write_spi_interface_options_flags") {
// 	// 	CFA835::InterfaceOption flags = CFA835::InterfaceOption::CFA835_WILL_SEND_ERRORS_FROM_COMMANDS_RECEIVED_ON_ANY_INTERFACE;
// 	// 	CFA835::SpiCpol cpol = CFA835::SpiCpol::FIRST_EDGE;
// 	// 	CFA835::SpiCpha cpha = CFA835::SpiCpha::POLARITY_HIGH;
// 	// 	CFA835::BitFirst bit_first = CFA835::BitFirst::MSB_FIRST;
// 	// 	uint8_t reserve = 10; // random number.
// 	// 	CFA835::SPIInterfaceOption options(flags, cpol, cpha, bit_first, reserve);
// 	// 	device->write_spi_interface_options_flags(options);
// 	// }

// 	// SECTION("Test write_i2c_interface_options_flags") {
// 	// 	CFA835::InterfaceOption flags = CFA835::InterfaceOption::CFA835_WILL_SEND_ERRORS_FROM_COMMANDS_RECEIVED_ON_ANY_INTERFACE;
// 	// 	uint8_t i2c_address = 0x00;
// 	// 	CFA835::I2CBusSpeed speed = CFA835::I2CBusSpeed::_400Khz;
// 	// 	CFA835::S2CInterfaceOption options(flags, i2c_address, speed);
// 	// 	device->write_i2c_interface_options_flags(options);
// 	// }
// }

// TEST_CASE("Test read_gpio", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	SECTION("Test read_gpio READ") {
// 		CFA835::GPIOIndex index = CFA835::GPIOIndex::LED_0_GREED;
// 		CFA835::GPIOResult result = device->read_gpio(index);

// 		REQUIRE(index == result.index);
// 	}
// }

// TEST_CASE("Test read_adc", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	SECTION("Test read_adc READ") {
// 		CFA835::ADCIndex index = CFA835::ADCIndex::ADC_0;
// 		CFA835::ADCResult result = device->read_adc(index);

// 		REQUIRE(index == result.index);
// 	}
// }

// TEST_CASE("Test configure_gpio", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	uint8_t pin_output_state = 0; // Random number 0 -> 100
// 	while (pin_output_state++ <= 100)
// 	 {
// 		CFA835::GPIOIndex index = CFA835::GPIOIndex::LED_3_RED;
// 		device->configure_gpio(index, pin_output_state);
// 		// For debug purpose. Because it runs so fast and we cannot see anything.
// 		// std::this_thread::sleep_for(std::chrono::milliseconds(300));
// 	}
// }

// TEST_CASE("Test load_font_from_micro_sd_card", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	uint8_t option_flags = 0;

// 	device->load_font_from_micro_sd_card(1, option_flags, "monaco9.cff");
// }

// TEST_CASE("Test print_custom_font_to_display", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	Vector2i position = Vector2i(2, 58);

// 	device->print_custom_font_to_display(1, position, "Hello world");
// }

// TEST_CASE("Test print_custom_font_to_display again", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	Vector2i position = Vector2i(0, 0);
// 	uint8_t option_flags = 0;

// 	device->load_font_from_micro_sd_card(1, option_flags, "big.cff");
// 	device->print_custom_font_to_display(1, position, "OPSWAT");
// 	position = Vector2i(2, 30);
// 	device->print_custom_font_to_display(1, position, "OPSWAT");
// }

// TEST_CASE("Test open_micro_sd_file", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);

// 	uint8_t option_read = 1;

// 	device->open_micro_sd_file(option_read, "big.cff");
// }

// TEST_CASE("Test close_current_opened_micro_sd_file", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);

// 	device->close_current_opened_micro_sd_file();
// }

// TEST_CASE("Test micro_sd_file_write_data", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	std::string data = "This is some random text written using file write command";
// 	uint8_t option_truncate = 2;

// 	device->open_micro_sd_file(option_truncate, "sometext.txt");
// 	device->micro_sd_file_position_seek(0);
// 	device->micro_sd_file_write_data(std::vector<uint8_t>(data.begin(), data.end()));
// 	size_t num_bytes = data.size();
// 	device->micro_sd_file_position_seek(0);
// 	auto response = device->micro_sd_file_read_data(num_bytes);
// 	REQUIRE(data == std::string(response.begin(), response.end()));
// 	device->close_current_opened_micro_sd_file();
// }

// TEST_CASE("Test micro_sd_file_delete", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	uint8_t option_truncate = 3;
// 	device->open_micro_sd_file(option_truncate, "sometext.txt");
// 	// device->micro_sd_file_delete("sometext.txt");
// }

// TEST_CASE("Test send_bmp_to_display_from_host", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);

// 	device->send_bmp_to_display_from_host("/opt/ometadefender-drive/resources/cf/images/AutoCoutdown.bmp", Vector2i(0,0));
// }

// TEST_CASE("Test pixel_data", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	Vector2i position = Vector2i(121, 33);
// 	uint8_t shade = 255;

// 	device->set_pixel_data(position, shade);
// 	uint8_t current_value = device->get_pixel_data(position);

// 	REQUIRE(shade == current_value);
// }

// TEST_CASE("Test draw_a_line", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	Vector2i start = Vector2i(0, 20);
// 	Vector2i end = Vector2i(243, 67);
// 	uint8_t shade = 255;

// 	device->draw_a_line(start, end, shade);
// }

// TEST_CASE("Test draw_a_rectangle", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	Vector2i start = Vector2i(0, 0);
// 	Vector2i size = Vector2i(243, 67);
// 	uint8_t shade = 255;
// 	uint8_t fill = 100;

// 	device->draw_a_rectangle(start, size, shade, fill);
// }

// TEST_CASE("Test draw_a_circle", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	Vector2i position = Vector2i(121, 33);
// 	uint8_t radius = 33;
// 	uint8_t shade = 255;
// 	uint8_t fill = 100;

// 	device->draw_a_circle(position, radius, shade, fill);
// }

// TEST_CASE("Test set_manual_flush_graphic_buffer", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	bool is_manual_flush = true;

// 	device->set_manual_flush_graphic_buffer(is_manual_flush);
// }

// TEST_CASE("Test set_gamma_correction", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	bool enable_gamma_correction = true;

// 	device->set_gamma_correction(enable_gamma_correction);
// }

// TEST_CASE("Test flush_buffer", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);

// 	device->flush_buffer();
// }

// TEST_CASE("Test upload_file", "[CFA835]") {
// 	CFA835 *device = static_cast<CFA835 *>(DeviceManager::QUEUE_FOR_TESTING);
// 	const std::string path = "/opt/ometadefender-drive/assets/fonts/";
// 	const int8_t truncates = 2;

// 	for (const auto &entry : std::filesystem::directory_iterator(path)) {
// 		if (std::filesystem::is_regular_file(entry.path())) {
// 			std::string filename = entry.path().filename();
// 			device->open_micro_sd_file(truncates, filename);
// 			device->close_current_opened_micro_sd_file();
// 			// device->micro_sd_file_position_seek(0);
// 			// device->micro_sd_file_delete(filename);
// 			// device->upload_file(entry.path());
// 		}
// 	}
// }
