#include "Softbody.h"

Softbody::Softbody(glm::vec2 position, glm::vec2 velocity, glm::vec2 force, float mass, ImVec4 colour, bool isStatic, int noPoints, float radius, float springConstant, float dampeningFactor) :
	Body(position, velocity, force, mass, colour, isStatic), noPoints(noPoints), radius(radius), springConstant(springConstant), dampeningFactor(dampeningFactor) {

	points = std::vector<Point>();
	springs = std::vector<Spring>();

	setupPoints();
	setupSprings();
}

void Softbody::setupPoints() {

	// Iterates through angles around a cirlce
	for (float i = 0; i < 2 * 3.141; i += 2 * 3.141 / noPoints) {
		
		// Position on the circle
		glm::vec2 pos = glm::vec2(cos(i), sin(i)) * radius;
		// Creates a circle at that point with similar properties to the softbody
		Circle c = Circle(5, pos + position, velocity, force, mass / noPoints, colour, isStatic);
		// Make the circle a shared pointer and add it to points
		points.push_back(std::make_shared<Circle>(c));

	}
}