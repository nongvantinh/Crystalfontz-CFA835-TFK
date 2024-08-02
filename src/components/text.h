#ifndef TEXT_H
#define TEXT_H

#include "component.h"

#include <memory>

class Text : public Component {
private:
	std::string font;
	std::string text;

public:
	Text();
	void initialize() override;
	void update(float p_delta) override;

	void set_text(const std::string p_text);
	std::string get_text() const;

	void set_font(const std::string p_font);
	std::string get_font() const;

	Vector2i get_minimum_size() const override;
};

#endif // TEXT_H