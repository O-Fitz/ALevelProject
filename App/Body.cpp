#include "Body.h"
#include "Renderer.h"

void Body::update(double dt) {

}

void Body::render(Renderer* renderer) {
	std::cout << "BODY RENDER" << std::endl;
}

Projection Body::project(glm::vec2 axis) {
	return Projection(axis);
}
