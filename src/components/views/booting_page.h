#ifndef BOOTING_PAGE_H
#define BOOTING_PAGE_H

#include "component.h"

class Text;
class BootingPage : public Component {
public:
	void initialize() override;

private:
	std::shared_ptr<Text> message;
};

#endif // BOOTING_PAGE_H