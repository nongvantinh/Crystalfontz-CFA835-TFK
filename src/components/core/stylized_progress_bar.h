#ifndef STYLIZED_PROGRESS_BAR_H
#define STYLIZED_PROGRESS_BAR_H

#include "progress_bar.h"

class StylizedProgressBar : public ProgressBar {
public:
	StylizedProgressBar() { set_max_value(7); }
	StylizedProgressBar(int32_t p_max_value) { set_max_value(p_max_value); }
	void initialize() override;
	void update(float p_delta) override;
	void set_max_value(uint32_t p_value) override;
	Vector2i get_minimum_size() const override;

private:
	void increment_index();
	void decrement_index();
	size_t current_index = 0;
	float threshold = 0.3f;
	float timer = 0.0f;
	int32_t min_space = 5;
	ShapeCategory shape = ShapeCategory::PARALLELOGRAM_RIGHT;
};

#endif // STYLIZED_PROGRESS_BAR_H