#ifndef NO_THREATS_FOUND_PAGE_H
#define NO_THREATS_FOUND_PAGE_H

#include "component.h"

class Text;
class NoThreatsFoundPage : public Component {
public:
	void initialize() override;
	void update(UNUSED_PARAM float p_delta) override;

private:
	float timer;
	std::shared_ptr<Text> message;
};

#endif // NO_THREATS_FOUND_PAGE_H