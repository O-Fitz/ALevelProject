#pragma once
#include "glm/glm.hpp"

struct Projection {
	double min;
	double max;
	glm::vec2 axis;

	Projection(glm::vec2 axis);
	Projection(glm::vec2 axis, double min, double max);

	bool overlap(const Projection& proj2);
};

namespace Collision {

	

}