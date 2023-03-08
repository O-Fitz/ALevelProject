#pragma once
#include "glm/glm.hpp"
#include "Circle.h"
#include <memory>

class Spring {
public:
	Spring();
	Spring(std::shared_ptr<Circle> point1, std::shared_ptr<Circle> point2, double naturalLength, double springConstant);

	void update(double dt);

private:

	std::shared_ptr<Circle> points[2];
	double naturalLength;
	double springConstant;

};