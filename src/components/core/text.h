#ifndef TEXT_H
#define TEXT_H

#include "component.h"

class Text : public Component {
private:
	Font font;
	int32_t font_size;
	std::string text;

public:
	Text();
	Text(const std::string & p_text);
	void initialize() override;
	void update(float p_delta) override;

	void set_text(const std::string &p_text);
	std::string get_text() const;
	void set_font(const Font &p_font);
	Font get_font() const;
	void set_font_size(const int32_t &p_size);
	int32_t get_font_size() const;

	Vector2i get_minimum_size() const override;
};

#endif // TEXT_H