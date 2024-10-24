#ifndef SCANNING_PAGE_H
#define SCANNING_PAGE_H

#include "component.h"

class Text;
class GeometryShape;
class ScanningPage : public Component {
public:
	void initialize() override;
	void update(UNUSED_PARAM float p_delta) override;

private:
	float timer;
	std::shared_ptr<Text> message;
	std::shared_ptr<GeometryShape> left_arrow;
	std::shared_ptr<GeometryShape> down_arrow;
};

#endif // SCANNING_PAGE_H