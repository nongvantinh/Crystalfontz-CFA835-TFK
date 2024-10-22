#ifndef CFA835_H
#define CFA835_H
#include "crystalfontz_device.h"

#include <boost/asio.hpp>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>

enum class KeyCode;
class CrystalfontzMessage;
class Vector2i;
/*
 * Model Classification Information:
 *   - Brand: Crystalfontz America, Inc.
 *   - Model Identifier: 835
 *   - Blacklight Type & Color: LED
 *   - Fluid Type, Image (Positive or Negative) & LCD Glass Color: FSTN, Positive, Neutral
 *   - Polarizer Film Type, Temperature Range, & Viewing Direction (O'clock): Transflective, Wide Temperature -20°C to +70°C, 12:00
 */

class CFA835 : public CrystalfontzDevice {
public:
	// Refer to the Crystalfontz CFA835 dataset section 8.11 on command codes.
	enum class CommandCode {
		PING = 0x00,
		GET_MODULE_INFOMATION = 0x01,
		WRITE_USER_FLASH_AREA = 0x02,
		READ_USER_FLASH_AREA = 0x03,
		STORE_CURRENT_STATE_AS_BOOT_STATE = 0x04,
		RESTART = 0x05,
		CLEAR_DISPLAY = 0x06,

		SPECIAL_CHARACTER_BITMAPS = 0x09,

		DISPLAY_CURSOR_POSITION = 0x0B,
		CURSOR_STYLE = 0x0C,
		CONTRAST = 0x0D,
		DISPLAY_AND_KEYPAD_BACKLIGHTS = 0x0E,

		KEYPAD_REPORTING = 0x17,
		READ_KEYPAD_OR_POLLED_MODE = 0x18,

		ATX_FUNCTIONALITY = 0x1C,
		WATCHDOG = 0x1D,
		WRITE_TEXT_TO_THE_DISPLAY = 0x1F,
		READ_TEXT_FROM_THE_DISPLAY = 0x20,
		INTERFACE_OPTIONS = 0x21,
		GPIO_PIN_CONFIGURATION = 0x22,
		INTERFACE_BRIDGE = 0x24,
		CFA_FBSCAB_COMMAND_GROUP = 0x25,
		CUSTOM_FONTS_COMMAND_GROUP = 0x26,
		MICROSD_FILE_OPERATIONS_COMMAND_GROUP = 0x27,
		DISPLAY_GRAPHIC_OPTIONS_COMMAND_GROUP = 0x28,
		VIDEO_PLAYBACK_CONTROL_COMMAND_GROUP = 0x29,

		DEBUGGING = 0x3E,
		RESPONSE_MESSAGE = 0x40,
		KEY_ACTIVITY = 0x80,
	};

	enum class CursorStyle {
		HIDDEN,
		BLINKING_BLOCK,
		UNDERSCORE,
		BLINGKING_BLOCK_PLUS_UNDERSCORE,
		INVERTING_BLICKING_BLOCK,
	};

	enum class KeyActivity {
		KEY_UP_PRESS = 1,
		KEY_DOWN_PRESS,
		KEY_LEFT_PRESS,
		KEY_RIGHT_PRESS,
		KEY_ENTER_PRESS,
		KEY_EXIT_PRESS,

		KEY_UP_RELEASE,
		KEY_DOWN_RELEASE,
		KEY_LEFT_RELEASE,
		KEY_RIGHT_RELEASE,
		KEY_ENTER_RELEASE,
		KEY_EXIT_RELEASE,
	};

	enum class AtxBitmasks {
		AUTO_POLARITY,
		RESTART_INVERT,
		POWER_INVERT,
		LEDS_MIMIC_HOST_POWER,
		MODULE_MIMIC_HOST_POWER,
		KEYPAD_RESTART,
		KEYPAD_POWER_ON,
		KEYPAD_POWER_OFF,
		COUNT
	};

	enum class InterfaceOption {
		ENABLE_INTERFACE = 1 << 0, // bit 0
		COMMAND_INTERPRETER_ENABLED = 1 << 1, // bit1
		CFA835_WILL_SEND_REPORT_PACKETS_ON_THIS_INTERFACE = 1 << 2, // bit2
		CFA835_WILL_SEND_ERRORS_FROM_COMMANDS_RECEIVED_ON_THIS_INTERFACE = 1 << 3, // bit3
		CFA835_WILL_SEND_ERRORS_FROM_COMMANDS_RECEIVED_ON_ANY_INTERFACE = 1 << 4, // bit4
		COUNT
	};

	enum class BaudRate {
		_19200,
		_115200,
		_9600,
		COUNT
	};

	struct SerialInterfaceOption {
		InterfaceOption option_flags;
		BaudRate baud_rate;
		SerialInterfaceOption(InterfaceOption p_option_flags, BaudRate p_rate) :
				option_flags(p_option_flags), baud_rate(p_rate) {};
	};

	enum class SpiCpol {
		FIRST_EDGE,
		SECOND_EDGE,
		COUNT
	};

	enum class SpiCpha {
		POLARITY_LOW,
		POLARITY_HIGH,
		COUNT
	};

	enum class BitFirst {
		MSB_FIRST,
		LSB_FIRST,
		COUNT
	};

	struct SPIInterfaceOption {
		InterfaceOption option_flags;
		SpiCpol cpol;
		SpiCpha cpha;
		BitFirst bit_first;
		uint8_t reserved;
		SPIInterfaceOption(InterfaceOption p_option_flags, SpiCpol p_cpol, SpiCpha p_cpha, BitFirst p_bit_first, uint8_t p_reserved) :
				option_flags(p_option_flags), cpol(p_cpol), cpha(p_cpha), bit_first(p_bit_first), reserved(p_reserved) {};
	};

	enum class I2CBusSpeed {
		_100Khz,
		_400Khz,
		COUNT
	};

	struct S2CInterfaceOption {
		InterfaceOption option_flags;
		uint8_t i2c_address;
		I2CBusSpeed speed;
		S2CInterfaceOption(InterfaceOption p_option_flags, uint8_t p_i2c_address, I2CBusSpeed p_speed) :
				option_flags(p_option_flags), i2c_address(p_i2c_address), speed(p_speed) {};
	};

	enum class GPIOIndex {
		GPIO_0,
		GPIO_1,
		GPIO_2,
		GPIO_3,
		GPIO_4,
		LED_3_GREED,
		LED_3_RED,
		LED_2_GREED,
		LED_2_RED,
		LED_1_GREED,
		LED_1_RED,
		LED_0_GREED,
		LED_0_RED,
		GPIO_5,
		GPIO_6,
		GPIO_7,
		GPIO_8,
		GPIO_9,
		GPIO_10,
		GPIO_11,
		GPIO_12,
	};

	enum class ADCIndex {
		ADC_0 = static_cast<int>(GPIOIndex::GPIO_5),
		ADC_1 = static_cast<int>(GPIOIndex::GPIO_6),
	};

	struct GPIOResult {
		GPIOIndex index;
		bool pin_state_s;
		bool pin_state_f;
		bool pin_state_r;
		uint8_t pin_pwm_output_value;
		uint8_t pin_function_select;
		uint8_t pin_drive_mode;
		GPIOResult(GPIOIndex p_index, bool p_pin_state_s, bool p_pin_state_f, bool p_pin_state_r,
				uint8_t p_pin_pwm_output_value, uint8_t p_pin_function_select, uint8_t p_pin_drive_mode) :
				index(p_index), pin_state_s(p_pin_state_s), pin_state_f(p_pin_state_f), pin_state_r(p_pin_state_r), pin_pwm_output_value(p_pin_pwm_output_value), pin_function_select(p_pin_function_select), pin_drive_mode(p_pin_drive_mode) {};
	};

	struct ADCResult {
		ADCIndex index;
		uint16_t average;
		uint16_t min_value;
		uint16_t max_value;
		ADCResult(ADCIndex p_index, uint16_t p_average, uint16_t p_min_value, uint16_t p_max_value) :
				index(p_index), average(p_average), min_value(p_min_value), max_value(p_max_value) {};
	};

	struct KeypadEvent {
		KeyActivity key_press_mask;
		KeyActivity key_release_mask;
		KeypadEvent(KeyActivity p_key_press_mask, KeyActivity p_key_release_mask) :
				key_press_mask(p_key_press_mask),
				key_release_mask(p_key_release_mask) {}
	};

	bool initialize() override;
	void discard_initial_data();
	std::string ping(const std::string &p_message);
	std::string get_hardware_and_firmware_info();
	std::string get_serial_number();
	bool write_user_flash_area(const std::vector<uint8_t> &p_data);
	std::vector<uint8_t> read_user_flash_area(uint8_t p_num_bytes);
	bool store_current_state_as_boot_state();
	// Restart.
	bool reload_boot_settings();
	bool restart_host();
	bool power_off_host();
	bool cfa835_restart();
	bool cfa835_restore_default_settings();
	bool clear_display();
	std::vector<uint8_t> read_special_character_bitmaps(uint8_t p_index);
	bool write_special_character_bitmaps(uint8_t p_index, const std::vector<uint8_t> &p_bitmap);
	Vector2i show_cursor();
	void set_cursor_position(Vector2i p_position);
	CursorStyle get_cursor_style();
	void set_cursor_style(CursorStyle p_style);
	uint8_t get_constrat();
	void set_constrat(uint8_t p_constrat_setting);
	uint8_t get_display_brightness();
	void set_display_brightness(uint8_t p_display_brightness);
	uint8_t get_keypad_brightness();
	void set_keypad_brightness(uint8_t p_keypad_brightness);
	KeypadEvent get_key_events();
	void set_key_events(KeyCode p_keypad_press_mask, KeyCode p_keypad_release_mask);

	std::vector<uint8_t> keypad_reporting(const std::vector<uint8_t> &p_data);
	std::vector<uint8_t> read_keypad_or_polled_mode();
	// std::vector<uint8_t> atx_functionality(const std::vector<uint8_t> &p_data);
	// std::vector<uint8_t> watchdog(const std::vector<uint8_t> &p_data);
	void write_text_to_the_display(uint8_t p_column_position, uint8_t p_row_position, const std::string &p_text);
	std::string read_text_from_the_display(uint8_t p_column_position, uint8_t p_row_position, uint8_t p_length);
	// Interface Options: Doesn't work yet, needs to investigate.
	// CFA835::SerialInterfaceOption read_serial_interface_option_flags();
	// CFA835::InterfaceOption read_usb_interface_options_flags();
	// CFA835::SPIInterfaceOption read_spi_interface_options_flags();
	// CFA835::S2CInterfaceOption read_i2c_interface_options_flags();
	// void write_serial_interface_option_flags(CFA835::SerialInterfaceOption p_options);
	// void write_usb_interface_options_flags(CFA835::InterfaceOption p_options);
	// void write_spi_interface_options_flags(CFA835::SPIInterfaceOption p_options);
	// void write_i2c_interface_options_flags(CFA835::S2CInterfaceOption p_options);
	//  GPIO Pin Configuration (including on-board LEDs and ADC inputs)
	// GPIOResult read_gpio(GPIOIndex p_index);
	// ADCResult read_adc(ADCIndex p_index);
	void configure_gpio(GPIOIndex p_gpio, uint8_t p_pin_output_state);
	void load_font_from_micro_sd_card(uint8_t p_font_slot, uint8_t p_option_flags, std::string p_filename);
	void print_custom_font_to_display(uint8_t p_font_slot, Vector2i p_position, std::string p_text);
	void open_micro_sd_file(uint8_t p_option, std::string p_filename);
	void close_current_opened_micro_sd_file();
	void micro_sd_file_position_seek(size_t p_location);
	std::vector<uint8_t> micro_sd_file_read_data(uint8_t p_num_bytes);
	void micro_sd_file_write_data(std::vector<uint8_t> p_data);
	void micro_sd_file_delete(std::string p_filename);

	void set_manual_flush_graphic_buffer(bool p_enable);
	void set_gamma_correction(bool p_enable);
	void flush_buffer();
	void send_bmp_to_display_from_host(const std::vector<uint8_t> &p_data, const Vector2i &p_position, const Vector2i &p_size);
	void send_bmp_to_display_from_host(const std::string &p_bmp_path, const Vector2i &p_position);
	void set_pixel_data(Vector2i p_position, uint8_t p_shaded);
	uint8_t get_pixel_data(Vector2i p_position);
	void draw_a_line(Vector2i p_start_position, Vector2i p_end_position, uint8_t p_line_shade = 255);
	void draw_a_rectangle(Vector2i p_start_position, Vector2i p_size, uint8_t p_line_shade = 255, uint8_t p_fill_shade = 0);
	void draw_a_circle(Vector2i p_position, uint8_t p_radius, uint8_t p_line_shade = 255, uint8_t p_fill_shade = 0);

	CFA835(std::string p_device);
	~CFA835() override;

	void upload_file(std::string p_path) override;

	static const std::string TEST_CASE_TAG;
	static const std::string DEVICE_IDENTIFIER;
	static const uint8_t MAX_PACKAGE_SIZE;

	std::function<void(KeyActivity)> on_key_activity_event;

private:
	uint8_t read_uint8_t(boost::system::error_code &ec, int p_timeout_seconds = 5);
	uint16_t read_uint16(boost::system::error_code &ec, int p_timeout_seconds = 5);
	uint8_t send_data(const std::vector<uint8_t> &p_data);
	uint8_t send_message(CFA835::CommandCode p_type, const std::vector<uint8_t> &p_data);
	void read_package(int p_timeout_seconds = 5);
	void process_package(CommandCode command, const std::vector<uint8_t> &data, uint16_t crc);
	std::vector<uint8_t> receive_message(CFA835::CommandCode p_command, size_t p_length);
	uint16_t get_crc(std::vector<uint8_t>::const_iterator p_begin, std::vector<uint8_t>::const_iterator p_end);

private:
	static const uint8_t MIN_PACKAGE_SIZE;
	static const uint8_t MAX_RETRY;
	static const uint8_t SCREEN_WIDTH;
	static const uint8_t SCREEN_HEIGHT;

	static const uint16_t CRCTable[];
	boost::asio::io_context io_context;
	std::thread receive_thread;
	boost::asio::serial_port serial;
	std::mutex packages_mutex = std::mutex();
	std::condition_variable packages_cv;
	std::queue<std::pair<CommandCode, std::vector<uint8_t>>> packages;

	bool is_file_opened;
};
CFA835::InterfaceOption operator|(CFA835::InterfaceOption lhs, CFA835::InterfaceOption rhs);

#endif // CFA835_H