#include "Simulation.h"

Simulation::Simulation() {
	bodies = std::vector<Body>();
}

Simulation::~Simulation() {

}

void Simulation::update(double dt) {

	for (auto body : bodies) {

		body.update(dt);

	}

	checkCollisions();

}

void Simulation::setSimulationSize(glm::vec2 newSize) {
	simSize = newSize;
}

glm::vec2 Simulation::getSimulationSize() {
	return simSize;
}

std::vector<Body> Simulation::getBodies() {
	return bodies;
}

void Simulation::checkCollisions() {

}