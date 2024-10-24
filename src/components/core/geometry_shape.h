#ifndef GEOMETRY_SHAPE_H
#define GEOMETRY_SHAPE_H

#include "component.h"

class GeometryShape : public Component {
public:
	GeometryShape(const ShapeCategory& p_shape_category);
	void update(float p_delta) override;
	Vector2i get_minimum_size() const override;

private:
	ShapeCategory shape;
};

#endif // GEOMETRY_SHAPE_H