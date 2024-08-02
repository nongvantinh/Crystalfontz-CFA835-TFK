#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include "component.h"

#include <memory>

class ProgressBar : public Component {
private:
	uint32_t value;
	uint32_t max_value;

public:
	void set_value(uint32_t p_value);
	uint32_t get_value()const;

	virtual void set_max_value(uint32_t p_value);
	uint32_t get_max_value()const;
};

#endif // PROGRESS_BAR_H