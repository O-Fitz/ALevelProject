#include "Simulation.h"
#include "Renderer.h"

Simulation::Simulation() {
	bodies = std::vector<PBody>();

	glm::vec2 pos = glm::vec2(100, 100);
	glm::vec2 vel = glm::vec2(100, 100);

	Circle c1 = Circle(20, pos, vel, glm::vec2(0, 0), 1, ImVec4(0.0f, 1.0f, 1.0f, 1.0f));
	Square s2 = Square(20, pos, glm::vec2(0, 0), glm::vec2(0, 0), 1, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));

	bodies.push_back(std::make_shared<Circle>(c1));
	bodies.push_back(std::make_shared<Square>(s2));


}

Simulation::~Simulation() {

}

void Simulation::update(double dt) {

	for (int i = 0; i < bodies.size(); i++) {

		bodies[i]->update(dt);

	}

	checkCollisions();

}

void Simulation::setSimulationSize(glm::vec2 newSize) {
	simSize = newSize;
}

glm::vec2 Simulation::getSimulationSize() {
	return simSize;
}

std::vector<PBody> Simulation::getBodies() {
	return bodies;
}

void Simulation::checkCollisions() {
	
}