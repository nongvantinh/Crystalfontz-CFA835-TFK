#ifndef AUTO_FULL_SCAN_PAGE_H
#define AUTO_FULL_SCAN_PAGE_H

#include "component.h"

class Text;
class AutoFullScanPage : public Component {
public:
	void initialize() override;
	void update(UNUSED_PARAM float p_delta) override;

private:
	float timer;
	std::shared_ptr<Text> message;
};

#endif // AUTO_FULL_SCAN_PAGE_H