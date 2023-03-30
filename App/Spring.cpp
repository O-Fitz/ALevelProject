#include "Spring.h"
#include "Renderer.h"
#include "Utilities.h"

#include <iostream>

Spring::Spring() {

}

Spring::Spring(std::shared_ptr<Circle> point1, std::shared_ptr<Circle> point2, float naturalLength, float springConstant, float dampeningFactor) :
	naturalLength(naturalLength), springConstant(springConstant), dampeningFactor(dampeningFactor) {
	points[0] = point1;
	points[1] = point2;
}

void Spring::update(double dt) {

	if (points[0]->getStatic() && points[1]->getStatic()) {
		return;
	}

	glm::vec2 dif = points[0]->getPostition() - points[1]->getPostition();
	glm::vec2 force = springConstant * (glm::length(dif) - naturalLength) * glm::normalize(dif);

	if (points[0]->getStatic()) {
		points[1]->applyImpulse((force - points[1]->getVelocity() * dampeningFactor) * (float)dt);
	}
	else if (points[1]->getStatic()) {
		points[0]->applyImpulse((force - points[0]->getVelocity() * dampeningFactor) * -(float)dt);
	}
	else {
		glm::vec2 relVelocity = points[1]->getVelocity() - points[0]->getVelocity();
		glm::vec2 newForce = force - relVelocity * dampeningFactor;
		points[0]->applyImpulse(newForce * -0.5f * (float)dt);
		points[1]->applyImpulse(newForce * 0.5f * (float)dt);
	}
}

std::vector<glm::vec2> Spring::getPointsPos() {

	std::vector<glm::vec2> ret = std::vector<glm::vec2>();

	ret.push_back(points[0]->getPostition());
	ret.push_back(points[1]->getPostition());

	return ret;

}

void Spring::render(Renderer* renderer, ImVec4 colour) {
	renderer->drawLine(points[0]->getPostition(), points[1]->getPostition(), colour);
}

std::string Spring::save() {
	std::ostringstream os;
	os << utilities::to_str(points[0]) << " ";
	os << utilities::to_str(points[1]) << " ";
	os << utilities::to_str(naturalLength) << " ";
	os << utilities::to_str(springConstant) << " ";
	os << utilities::to_str(dampeningFactor);
	return os.str();
}