#ifndef WORKFLOWS_PAGE_H
#define WORKFLOWS_PAGE_H

#include "component.h"

class WorkflowsPage : public Component {
private:
public:
	WorkflowsPage();
	~WorkflowsPage();
	void initialize() override;
	void shutdown() override;
	void update(UNUSED_PARAM float p_delta) override;
};

#endif //WORKFLOWS_PAGE_H