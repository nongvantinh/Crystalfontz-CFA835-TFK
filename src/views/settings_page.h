#ifndef SETTINGS_PAGE_H
#define SETTINGS_PAGE_H

#include "component.h"

class SettingsPage : public Component {
private:
public:
	SettingsPage();
	~SettingsPage();
	void initialize() override;
	void shutdown() override;
	void update(UNUSED_PARAM float p_delta) override;
};

#endif //SETTINGS_PAGE_H