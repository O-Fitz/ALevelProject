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

Projection Circle::project(glm::vec2 axis) {
	float val = glm::dot<float>(position, axis);
	std::cout << val-radius << "\n\n";
	return Projection(axis, val-radius, val+radius);
}

void Circle::print() {
	std::cout << "Circle" << std::endl;
}

std::vector<glm::vec2> Circle::getAxes() {
	return std::vector<glm::vec2>({ {1,0} });
}