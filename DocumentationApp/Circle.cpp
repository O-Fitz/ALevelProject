#include "Circle.h"
#include "Renderer.h"

Circle::Circle() :
	radius(1), Rigidbody() {

}

Circle::Circle(double rad, glm::vec2 pos, glm::vec2 vel, glm::vec2 Force, double Mass, ImVec4 col, bool stat) :
	radius(rad), Rigidbody(std::vector<glm::vec2>(), pos, vel, Force, Mass, col, stat) {

}

void Circle::render(Renderer* renderer) {
	renderer->renderCircle(position, radius, colour);
}

glm::vec2 Circle::project(glm::vec2 axis) {
	
	// Project position onto axis
	float dotProduct = glm::dot<float>(position, axis);

	// Add and subtract radius to find min and max
	return glm::vec2(dotProduct - radius, dotProduct + radius);
}

std::vector<glm::vec2> Circle::getAxes() {
	std::vector<glm::vec2>axes = std::vector<glm::vec2>();

	// Iterate through angles in radians
	for (float i = 0; i < 2 * 3.141; i += 2 * 3.141 / collisionAxis) {
		// x = sin(i)
		// y = cos(i)
		// Dont need to multiply by the radius as the axis is only a direction
		axes.emplace_back(sin(i), cos(i));
	}
	return axes;
}