#include "Simulation.h"
#include "Renderer.h"

Simulation::Simulation() {
	bodies = std::vector<Body*>();

	glm::vec2 pos = glm::vec2(100, 100);
	glm::vec2 vel = glm::vec2(10, 10);

	Circle* c1 = new Circle(20, pos, vel, glm::vec2(0, 0), 1, ImVec4(0.0f, 1.0f, 1.0f, 1.0f));

	bodies.push_back(c1);

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

std::vector<Body*> Simulation::getBodies() {
	return bodies;
}

void Simulation::checkCollisions() {
	
}