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