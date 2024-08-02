#ifndef CONFIRM_FULL_SCAN_PAGE_H
#define CONFIRM_FULL_SCAN_PAGE_H

#include "component.h"

class ConfirmFullScanPage : public Component {
public:
	ConfirmFullScanPage();
	~ConfirmFullScanPage();
	void initialize() override;
	void input_event(std::shared_ptr<InputEvent> p_input_event) override;

private:
};
#endif // CONFIRM_FULL_SCAN_PAGE_H