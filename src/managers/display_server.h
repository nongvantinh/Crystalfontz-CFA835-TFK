#ifndef DISPLAY_SERVER_H
#define DISPLAY_SERVER_H

#include <memory>

#include "math/vector2.h"
#include "math/vector2i.h"

struct FontDetails {
	std::string name;
	Vector2 lowercase_size;
	Vector2 uppercase_size;
	FontDetails() = default;
	FontDetails(std::string p_name, Vector2 p_lowercase_size, Vector2 p_uppercase_size) :
			name(p_name), lowercase_size(p_lowercase_size), uppercase_size(p_uppercase_size) {}
};

class BaseDevice;
class DisplayServer {
private:
	std::shared_ptr<BaseDevice> main_device;

public:
	DisplayServer();
	~DisplayServer();
	static DisplayServer *get_singleton();

	void set_main_display(std::shared_ptr<BaseDevice> p_device);
	std::shared_ptr<BaseDevice> get_main_display() const;

	void clear_display() const;
	void flush_graphic_buffer() const;

	Vector2i get_required_space_for_text(const std::string &p_font, const std::string &p_text) const;

	void set_pixel(Vector2i p_position, uint8_t p_intensity = 255);
	void draw_line(Vector2i p_start_position, Vector2i p_end_position, uint8_t p_thickness, uint8_t p_intensity = 255);
	void draw_rectangle(Vector2i p_top_left, Vector2i p_size, uint8_t p_border_intensity, uint8_t p_fill = 255);
	void draw_circle(Vector2i p_center, uint8_t p_radius, uint8_t p_border_thickness, uint8_t p_fill = 255);
	void draw_horizontal_line(int x1, int x2, int y, uint8_t p_intensity);
	void draw_triangle(Vector2i v1, Vector2i v2, Vector2i v3, uint8_t p_border_thickness, uint8_t p_fill_intensity, uint8_t p_border_intensity = 255);
	void draw_parallelogram(Vector2i p_top_left, Vector2i p_top_right, Vector2i p_bottom_left, Vector2i p_bottom_right, uint8_t p_border_thickness, uint8_t p_fill = 255);
	void print_text_to_display(std::string p_font, std::string p_text, Vector2i p_position);
	void print_bmp_to_display(const std::vector<uint8_t> &p_data, const Vector2i &p_position, const Vector2i &p_size);
};

#endif // DISPLAY_SERVER_H