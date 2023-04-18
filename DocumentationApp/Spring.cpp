#include "Spring.h"
#include "Renderer.h"
#include "Circle.h"

Spring::Spring(Point point1, Point point2, float naturalLength, float springConstant, float dampeningFactor) :
	point1(point1), point2(point2), naturalLength(naturalLength), springConstant(springConstant), dampeningFactor(dampeningFactor) {



}

void Spring::update(double dt) {

	if (point1->getStatic() && point2->getStatic()) {
		return;
	}

	// Calculate force using F=kx
	glm::vec2 difference = point1->getPosition() - point2->getPosition();
	glm::vec2 force = springConstant * (glm::length(difference) - naturalLength) * glm::normalize(difference);

	// Apply force subtract dampening factor * relative velocity
	if (point1->getStatic()) {
		// Relative velocity is velocity of point2
		point2->applyImpulse((force - point2->getVelocity() * dampeningFactor) * (float)dt);
	}
	else if (point2->getStatic()) {
		// Relative velocity is velocity of point1
		point1->applyImpulse((force - point1->getVelocity() * dampeningFactor) * (float)dt);
	}
	else {
		// Apply half of the force to both of the points
		glm::vec2 relVelocity = point2->getVelocity() - point1->getVelocity();
		glm::vec2 newForce = force - relVelocity * dampeningFactor;

		point1->applyImpulse(-0.5f * newForce * (float)dt);
		point2->applyImpulse(0.5f * newForce * (float)dt);
	}

}

void Spring::render(Renderer* renderer, ImVec4 colour) {
	renderer->drawLine(point1->getPosition(), point2->getPosition(), colour);
}