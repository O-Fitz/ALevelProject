#include "Simulation.h"

Simulation::Simulation() {
	bodies = std::vector<Body>();
}

Simulation::~Simulation() {

}

void Simulation::update(double dt) {
	for (int i = 0; i < bodies.size(); i++) {
		bodies[i].update(dt);
	}
}

std::vector<Body> Simulation::getBodies() {
	return bodies;
}

glm::vec2 Simulation::getSimulationSize() {
	return simSize;
}

void Simulation::setSimulationSize(glm::vec2 size) {
	simSize = size;
}