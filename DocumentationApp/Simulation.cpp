#include "Simulation.h"
#include "Renderer.h"

Simulation::Simulation() {
	bodies = std::vector<PBody>();

	glm::vec2 pos = glm::vec2(100, 100);
	glm::vec2 pos2 = glm::vec2(200, 200);
	glm::vec2 vel = glm::vec2(-100, 0);
	glm::vec2 vel2 = glm::vec2(0, 0);

	Circle c1 = Circle(20, pos, vel, glm::vec2(0, 0), 2, ImVec4(0, 255, 0, 255), false);
	Square s1 = Square(20, pos2, vel2, glm::vec2(0, 0), 2, ImVec4(255, 0, 0, 255), false);

	Square s2 = Square(20, pos2, vel2, vel2, 4, ImVec4(0, 255, 0, 255), true);
	Square s3 = Square(20, pos2+glm::vec2(100, 0), vel, vel2, 2, ImVec4(255, 0, 0, 255), false);
	
	bodies.push_back(std::make_shared<Square>(s2));
	bodies.push_back(std::make_shared<Square>(s3));

	//bodies.push_back(std::make_shared<Circle>(c1));
	//bodies.push_back(std::make_shared<Square>(s1));

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
			if (!checked[j]) {

				glm::vec2 MTV = checkCollision(collisionBodies[i], collisionBodies[j]);
				if (MTV != glm::vec2(0, 0)) {
					collisionResloution(collisionBodies[i], collisionBodies[j], MTV);
				}
			}
		}
	}
}

glm::vec2 Simulation::checkCollision(Body* b1, Body* b2) {
	// Find all of the axes and append them to one vector
	std::vector<glm::vec2> axes = b1->getAxes();
	std::vector<glm::vec2> b2axes = b2->getAxes();
	axes.insert(std::end(axes), std::begin(b2axes), std::end(b2axes));

	float overlap = -std::numeric_limits<float>::max();
	glm::vec2 MTV = glm::vec2(0, 0);

	// Iterate through axes
	for (const glm::vec2& axis : axes) {
		// Project shapes onto axis
		glm::vec2 p1 = b1->project(axis);
		glm::vec2 p2 = b2->project(axis);

		// If they don't overlap, Bodies dont collide
		if (p1.x > p2.y || p1.y < p2.x) {
			return glm::vec2(0, 0);
		}
		else {
			float d1 = p2[0] - p1[1];
			float d2 = p1[0] - p2[1];
			float distance = std::max(d1, d2);
			if (distance > overlap) {
				overlap = distance;
				MTV = glm::normalize(axis) * overlap;
			}
		}
	}

	return MTV;
}

void Simulation::collisionResloution(Body* b1, Body* b2, glm::vec2 MTV) {

	if (b1->getStatic() && b2->getStatic()) {
		return;
	}
	positionCorrection(b1, b2, MTV);
	impulseCalculation(b1, b2);
}

void Simulation::positionCorrection(Body* b1, Body* b2, glm::vec2 MTV) {

	

	if (b1->getStatic() && b2->getStatic()) {
		return;
	}


	float rMass1 = 1 / b1->getMass();
	float rMass2 = 1 / b2->getMass();

	glm::vec2 moveVector = MTV * glm::fvec1(1 / (rMass1 + rMass2));

	// Move objects

	if (b1->getStatic()) {
		b2->addPosition(moveVector * (rMass1 + rMass2));
	}
	else if (b2->getStatic()) {
		b1->addPosition(-moveVector * (rMass1 + rMass2));
	}
	else {
		b1->addPosition(moveVector * rMass1);
		b2->addPosition(-moveVector * rMass2);
	}

}

void Simulation::impulseCalculation(Body* b1, Body* b2) {

	glm::vec2 normal = glm::normalize(b2->getPostition() - b1->getPostition());

	if (b1->getStatic()) {
		glm::vec2 par = normal * (glm::dot(b2->getVelocity(), normal) / glm::dot(normal, normal));
		glm::vec2 perp = b2->getVelocity() - par;
		glm::vec2 impulse = (perp - par) * (elasticity * b2->getMass() * 2);
		b2->applyImpulse(impulse);
	}
	else if (b2->getStatic()) {
		glm::vec2 par = normal * (glm::dot(b1->getVelocity(), normal) / glm::dot(normal, normal));
		glm::vec2 perp = b1->getVelocity() - par;
		glm::vec2 impulse = (perp - par) * (elasticity * b1->getMass() * 2);
		b2->applyImpulse(impulse);
	}
	else {

		// Find the relative velocity of the objects
		glm::vec2 rv = b2->getVelocity() - b1->getVelocity();

		// Find the relative velocity along the normal
		float velAlongNormal = glm::dot(rv, normal);

		// Calculate the impulse scalar
		float j = -(1 + elasticity) * velAlongNormal;
		j /= 1 / b1->getMass() + 1 / b2->getMass();

		// Apply the impulse to the objects
		glm::vec2 impulse = normal * j;

		b1->applyImpulse(-impulse);
		b2->applyImpulse(impulse);
	}
}