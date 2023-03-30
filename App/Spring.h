#pragma once
#include "glm/glm.hpp"
#include "Circle.h"
#include <memory>

#ifndef Point
#define Point std::shared_ptr<Circle>
#endif // Point


class Spring {
public:
	Spring();
	Spring(Point point1, Point point2, float naturalLength, float springConstant, float dampeningFactor);

	void update(double dt);
	void render(Renderer* renderer, ImVec4 colour);

	std::vector<glm::vec2> getPointsPos();

	std::string save();

private:

	Point points[2];
	float naturalLength;
	float springConstant;
	float dampeningFactor;
};