#include "display_server.h"

#include <cctype>
#include <type_traits>

#include "font_string_names.h"
#include "hardwares/crystalfontz/cfa835.h"
#include "hardwares/crystalfontz/cff_reader.h"

DisplayServer::DisplayServer() {}

DisplayServer::~DisplayServer() {
}

DisplayServer *DisplayServer::get_singleton() {
	static DisplayServer self;
	return &self;
}

void DisplayServer::set_main_display(std::shared_ptr<BaseDevice> p_device) {
	main_device = p_device;
	if (std::shared_ptr<CFA835> cfa835 = std::dynamic_pointer_cast<CFA835>(main_device)) {
		cfa835->set_manual_flush_graphic_buffer(true);
	} else {
		std::cout << "basePtr does not point to any instance of BaseDevice." << std::endl;
	}
}

std::shared_ptr<BaseDevice> DisplayServer::get_main_display() const {
	return main_device;
}

void DisplayServer::clear_display() const {
	if (std::shared_ptr<CFA835> cfa835 = std::dynamic_pointer_cast<CFA835>(main_device)) {
		cfa835->clear_display();
	} else {
		std::cout << "basePtr does not point to any instance of BaseDevice." << std::endl;
	}
}

void DisplayServer::flush_graphic_buffer() const {
	if (std::shared_ptr<CFA835> cfa835 = std::dynamic_pointer_cast<CFA835>(main_device)) {
		cfa835->flush_buffer();
	} else {
		std::cout << "basePtr does not point to any instance of BaseDevice." << std::endl;
	}
}

Vector2i DisplayServer::get_required_space_for_text(const std::string &p_font, const std::string &p_text) const {
	if (std::shared_ptr<CFA835> cfa835 = std::dynamic_pointer_cast<CFA835>(main_device)) {
		CFFReader reader("/opt/ometadefender-drive/assets/fonts/" + p_font);

		return reader.get_space_required(p_text);
	} else {
		std::cout << "basePtr does not point to any instance of BaseDevice." << std::endl;
	}

	return Vector2i(0, 0);
}

void DisplayServer::set_pixel(Vector2i p_position, uint8_t p_intensity) {
	if (std::shared_ptr<CFA835> cfa835 = std::dynamic_pointer_cast<CFA835>(main_device)) {
		cfa835->set_pixel_data(p_position, p_intensity);
	} else {
		std::cout << "basePtr does not point to any instance of BaseDevice." << std::endl;
	}
}

void DisplayServer::draw_line(Vector2i p_start_position, Vector2i p_end_position, uint8_t, uint8_t p_intensity) {
	if (std::shared_ptr<CFA835> cfa835 = std::dynamic_pointer_cast<CFA835>(main_device)) {
		cfa835->draw_a_line(p_start_position, p_end_position, p_intensity);
	} else {
		std::cout << "basePtr does not point to any instance of BaseDevice." << std::endl;
	}
}

void DisplayServer::draw_rectangle(Vector2i p_top_left, Vector2i p_size, uint8_t p_border_intensity, uint8_t p_fill) {
	if (std::shared_ptr<CFA835> cfa835 = std::dynamic_pointer_cast<CFA835>(main_device)) {
		cfa835->draw_a_rectangle(p_top_left, p_size, p_border_intensity, p_fill);
	} else {
		std::cout << "basePtr does not point to any instance of BaseDevice." << std::endl;
	}
}

void DisplayServer::draw_circle(Vector2i p_center, uint8_t p_radius, uint8_t p_border_thickness, uint8_t p_fill) {
	if (std::shared_ptr<CFA835> cfa835 = std::dynamic_pointer_cast<CFA835>(main_device)) {
		cfa835->draw_a_circle(p_center, p_radius, p_border_thickness, p_fill);
	} else {
		std::cout << "basePtr does not point to any instance of BaseDevice." << std::endl;
	}
}

void DisplayServer::draw_horizontal_line(int x1, int x2, int y, uint8_t p_intensity) {
	if (x1 > x2) {
		std::swap(x1, x2);
	}
	for (int x = x1; x <= x2; ++x) {
		set_pixel(Vector2i(x, y), p_intensity);
	}
}

void DisplayServer::draw_triangle(Vector2i v1, Vector2i v2, Vector2i v3, uint8_t p_border_thickness, uint8_t p_fill_intensity, uint8_t p_border_intensity) {
	// Sort vertices by y-coordinate
	if (v1.y > v2.y)
		std::swap(v1, v2);
	if (v2.y > v3.y)
		std::swap(v2, v3);
	if (v1.y > v3.y)
		std::swap(v1, v3);

	// Calculate inverse slopes
	float invslope1 = (v2.y - v1.y != 0) ? float(v2.x - v1.x) / (v2.y - v1.y) : 0;
	float invslope2 = (v3.y - v1.y != 0) ? float(v3.x - v1.x) / (v3.y - v1.y) : 0;
	float invslope3 = (v3.y - v2.y != 0) ? float(v3.x - v2.x) / (v3.y - v2.y) : 0;

	// Fill the upper part of the triangle
	float curx1 = v1.x;
	float curx2 = v1.x;
	for (int y = v1.y; y <= v2.y; ++y) {
		draw_horizontal_line(curx1, curx2, y, p_fill_intensity);
		curx1 += invslope1;
		curx2 += invslope2;
	}

	// Fill the lower part of the triangle
	curx1 = v2.x;
	curx2 = v1.x + invslope2 * (v2.y - v1.y);
	for (int y = v2.y; y <= v3.y; ++y) {
		draw_horizontal_line(curx1, curx2, y, p_fill_intensity);
		curx1 += invslope3;
		curx2 += invslope2;
	}

	// Draw the triangle border
	draw_line(v1, v2, p_border_thickness, p_border_intensity);
	draw_line(v2, v3, p_border_thickness, p_border_intensity);
	draw_line(v3, v1, p_border_thickness, p_border_intensity);
}

void DisplayServer::draw_parallelogram(Vector2i p_top_left, Vector2i p_top_right, Vector2i p_bottom_left, Vector2i p_bottom_right, uint8_t p_border_thickness, uint8_t p_fill) {
	// Draw the four sides of the parallelogram
	draw_line(p_top_left, p_top_right, p_border_thickness, 255);
	draw_line(p_top_right, p_bottom_right, p_border_thickness, 255);
	draw_line(p_bottom_right, p_bottom_left, p_border_thickness, 255);
	draw_line(p_bottom_left, p_top_left, p_border_thickness, 255);

	int32_t min_y = std::min({ p_top_left.y, p_top_right.y, p_bottom_left.y, p_bottom_right.y });
	int32_t max_y = std::max({ p_top_left.y, p_top_right.y, p_bottom_left.y, p_bottom_right.y });

	for (int32_t y = min_y + 1; y < max_y; ++y) {
		// Calculate the intersection points with the left and right borders
		float left_x = p_top_left.x + (p_bottom_left.x - p_top_left.x) * (y - p_top_left.y) / (p_bottom_left.y - p_top_left.y);
		float right_x = p_top_right.x + (p_bottom_right.x - p_top_right.x) * (y - p_top_right.y) / (p_bottom_right.y - p_top_right.y);

		int32_t left_int_x = static_cast<int32_t>(left_x);
		int32_t right_int_x = static_cast<int32_t>(right_x);

		draw_line(Vector2i(left_int_x, y), Vector2i(right_int_x, y), 1, p_fill);
	}
}

void DisplayServer::print_text_to_display(std::string p_font, std::string p_text, Vector2i p_position) {
	if (std::shared_ptr<CFA835> cfa835 = std::dynamic_pointer_cast<CFA835>(main_device)) {
		uint8_t option_flags = 0;
		cfa835->load_font_from_micro_sd_card(1, option_flags, p_font);
		cfa835->print_custom_font_to_display(1, p_position, p_text);
	} else {
		std::cout << "basePtr does not point to any instance of BaseDevice." << std::endl;
	}
}

void DisplayServer::print_bmp_to_display(const std::vector<uint8_t> &p_data, const Vector2i &p_position, const Vector2i &p_size) {
	if (std::shared_ptr<CFA835> cfa835 = std::dynamic_pointer_cast<CFA835>(main_device)) {
		cfa835->send_bmp_to_display_from_host(p_data, p_position, p_size);
	} else {
		std::cout << "basePtr does not point to any instance of BaseDevice." << std::endl;
	}
}
