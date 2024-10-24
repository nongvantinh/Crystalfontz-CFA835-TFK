#ifndef CLASSIC_PROGRESS_BAR_H
#define CLASSIC_PROGRESS_BAR_H

#include "progress_bar.h"

#include <memory>

class ClassicProgressBar : public ProgressBar {
public:
	void initialize() override;
	void update(float p_delta) override;
};

#endif // CLASSIC_PROGRESS_BAR_H