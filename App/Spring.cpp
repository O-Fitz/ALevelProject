#include "Spring.h"

#include <iostream>

Spring::Spring() {

}

Spring::Spring(std::shared_ptr<Circle> point1, std::shared_ptr<Circle> point2, double naturalLength, double springConstant) :
	naturalLength(naturalLength), springConstant(springConstant) {
	points[0] = point1;
	points[1] = point2;
}

void Spring::update(double dt) {

	if (points[0]->getStatic() && points[1]->getStatic()) {
		return;
	}

	glm::vec2 dif = points[0]->getPostition() - points[1]->getPostition();
	glm::vec2 force = glm::fvec1(springConstant * (glm::length(dif) - naturalLength)) * glm::normalize(dif);

	if (points[0]->getStatic()) {
		points[1]->applyImpulse(force * glm::fvec1(dt));
	}
	else if (points[1]->getStatic()) {
		points[0]->applyImpulse(force * glm::fvec1(-dt));
	}
	else {
		points[0]->applyImpulse(force * glm::fvec1(-0.5 * dt));
		points[1]->applyImpulse(force * glm::fvec1(0.5 * dt));
	}
}