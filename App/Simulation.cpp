#include "Simulation.h"
#include "Renderer.h"

Simulation::Simulation() {
	bodies = std::vector<PBody>();

	glm::vec2 pos = glm::vec2(100, 100);
	glm::vec2 pos1 = glm::vec2(200, 200);
	glm::vec2 vel = glm::vec2(100, 100);

	//Circle c1 = Circle(20, pos, vel, glm::vec2(0, 0), 1, ImVec4(0.0f, 1.0f, 1.0f, 1.0f), false);
	Square s2 = Square(20, pos1, glm::vec2(0, 0), glm::vec2(0, 0), 1, ImVec4(1.0f, 1.0f, 0.0f, 1.0f), false);
	Square s1 = Square(20, pos, vel, glm::vec2(0, 0), 1, ImVec4(1.0f, 1.0f, 0.0f, 1.0f), false);

	bodies.push_back(std::make_shared<Square>(s1));
	bodies.push_back(std::make_shared<Square>(s2));


}

Simulation::~Simulation() {

}

void Simulation::update(double dt) {

	for (int i = 0; i < bodies.size(); i++) {

		bodies[i]->update(dt);

	}

	CollisionDetection();

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


void Simulation::CollisionDetection() {

	// Find all of the Bodies that can collide
	std::vector<Body*> collisionBodies = std::vector<Body*>();
	for (int i = 0; i < bodies.size(); i++) {
		std::vector<Body*> col = bodies[i]->getCollisionBodies();
		collisionBodies.insert(collisionBodies.end(), col.begin(), col.end());
	}

	// Check each body against every other body
	// TODO: Temporary, will add optimisation later
	std::vector<bool>checked = std::vector<bool>();
	for (int i = 0; i < collisionBodies.size(); i++) {
		checked.push_back(false);
	}

	for (int i = 0; i < collisionBodies.size(); i++) {
		checked[i] = true;
		for (int j = 0; j < collisionBodies.size(); j++){
			if (!checked[j]) {
				if (checkCollision(collisionBodies[i], collisionBodies[j])) {
					resolveCollision(collisionBodies[i], collisionBodies[j]);
				}
			}
		}
	}

}

bool Simulation::checkCollision(Body* b1, Body* b2) {

	// Find all of the axes and append them to one vector
	std::vector<glm::vec2> axes = std::vector<glm::vec2>();
	std::vector<glm::vec2> b1axes = b1->getAxes();
	std::vector<glm::vec2> b2axes = b2->getAxes();
	axes.insert(std::end(axes), std::begin(b1axes), std::end(b1axes));
	axes.insert(std::end(axes), std::begin(b2axes), std::end(b2axes));

	// Iterate through axes
	for (const glm::vec2& axis : axes) {
		// Project shapes onto axis
		Projection p1 = b1->project(axis);
		Projection p2 = b2->project(axis);

		std::cout << "P1:" << p1.min << p1.max << std::endl;
		std::cout << "P2:" << p2.min << p2.max << std::endl;
		std::cout << "\n\n";

		// If they don't overlap, Bodies dont collide
		if (!p1.overlap(p2)) {
			return false;
		}

	}

	return true;

}

void Simulation::resolveCollision(Body* b1, Body* p2) {
	std::cout << "COLLISION" << std::endl;
}