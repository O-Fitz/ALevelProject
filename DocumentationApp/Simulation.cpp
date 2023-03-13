#include "Simulation.h"
#include "Renderer.h"

Simulation::Simulation() {
	bodies = std::vector<PBody>();

	glm::vec2 pos = glm::vec2(100, 100);
	glm::vec2 pos2 = glm::vec2(200, 200);
	glm::vec2 vel = glm::vec2(100, 100);
	glm::vec2 vel2 = glm::vec2(0, 0);

	Circle c1 = Circle(20, pos, vel, glm::vec2(0, 0), 2, ImVec4(0, 255, 0, 255), false);
	Square s1 = Square(20, pos2, vel2, glm::vec2(0, 0), 2, ImVec4(255, 0, 0, 255), false);
	

	bodies.push_back(std::make_shared<Circle>(c1));
	bodies.push_back(std::make_shared<Square>(s1));

}

Simulation::~Simulation() {

}

void Simulation::update(double dt) {

	// Call update functions
	for (int i = 0; i < bodies.size(); i++) {
		bodies[i]->update(dt);
	}

	// Collision Detection
	collisionDetection();
}

std::vector<PBody> Simulation::getBodies() {
	return bodies;
}

glm::vec2 Simulation::getSimulationSize() {
	return simSize;
}

void Simulation::setSimulationSize(glm::vec2 size) {
	simSize = size;
}

void Simulation::collisionDetection() {
	// Find all of the Bodies that can collide
	std::vector<Body*> collisionBodies = std::vector<Body*>();
	for (int i = 0; i < bodies.size(); i++) {
		std::vector<Body*> col = bodies[i]->getCollisionBodies();
		collisionBodies.insert(collisionBodies.end(), col.begin(), col.end());
	}

	// Check each body against every other body
	// checked is the make sure that we dont compare 2 objects twice
	std::vector<bool>checked = std::vector<bool>();
	for (int i = 0; i < collisionBodies.size(); i++) {
		checked.push_back(false);
	}

	for (int i = 0; i < collisionBodies.size(); i++) {
		checked[i] = true;
		for (int j = 0; j < collisionBodies.size(); j++) {
			float min;
			if (!checked[j] && checkCollision(collisionBodies[i], collisionBodies[j])) {
				std::cout << "COLLISION" << std::endl;
			}
		}
	}
}

bool Simulation::checkCollision(Body* b1, Body* b2) {
	// Find all of the axes and append them to one vector
	std::vector<glm::vec2> axes = b1->getAxes();
	std::vector<glm::vec2> b2axes = b2->getAxes();
	axes.insert(std::end(axes), std::begin(b2axes), std::end(b2axes));


	// Iterate through axes
	for (const glm::vec2& axis : axes) {
		// Project shapes onto axis
		glm::vec2 p1 = b1->project(axis);
		glm::vec2 p2 = b2->project(axis);

		// If they don't overlap, Bodies dont collide
		if (p1.x > p2.y || p1.y < p2.x) {
			return false;
		}
	}

	return true;
}