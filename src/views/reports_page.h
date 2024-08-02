#ifndef REPORTS_PAGE_H
#define REPORTS_PAGE_H

#include "component.h"

class ReportsPage : public Component {
private:
public:
	ReportsPage();
	~ReportsPage();
	void initialize() override;
	void shutdown() override;
	void update(UNUSED_PARAM float p_delta) override;
};

#endif //REPORTS_PAGE_H