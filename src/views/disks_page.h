#ifndef DISKS_PAGE_H
#define DISKS_PAGE_H

#include "component.h"

class DisksPage : public Component {
private:
public:
	DisksPage();
	~DisksPage();
	void initialize() override;
	void shutdown() override;
	void update(UNUSED_PARAM float p_delta) override;
};

#endif // DISKS_PAGE_H