#pragma once

#include "Circle.h"
#include <memory>

#ifndef Point
#define Point std::shared_ptr<Circle>
#endif

class Spring {

public:

	Spring(Point point1, Point point2, float naturalLength, float springConstant, float dampeningFactor);

	void update(double dt);
	void render(Renderer* renderer, ImVec4 colour);

private:

	Point point1;
	Point point2;

	float naturalLength;
	float springConstant;
	float dampeningFactor;

};