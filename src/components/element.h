#ifndef ELEMENT_H
#define ELEMENT_H

#include "component.h"

#include <memory>

class Element : public Component {
private:
public:
	void initialize() override;
	void update(float p_delta) override;
};

#endif // ELEMENT_H