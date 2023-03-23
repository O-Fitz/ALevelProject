#include "Simulation.h"
#include "Renderer.h"

Simulation::Simulation() {
	bodies = std::vector<PBody>();

	glm::vec2 pos = glm::vec2(100, 100);
	glm::vec2 pos2 = glm::vec2(200, 200);
	glm::vec2 vel = glm::vec2(-100, 0);
	glm::vec2 vel2 = glm::vec2(200, 0);

	Circle c1 = Circle(20, pos, vel, glm::vec2(0, 0), 2, ImVec4(0, 255, 0, 255), false);
	Square s1 = Square(20, pos2, vel2, glm::vec2(0, 0), 2, ImVec4(255, 0, 0, 255), false);

	Square s2 = Square(20, pos2, vel2, vel2, 1, ImVec4(0, 255, 0, 255), false);
	Square s3 = Square(20, pos2+glm::vec2(100, 0), vel, vel2, 1, ImVec4(255, 0, 0, 255), false);
	
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

	float overlap = std::numeric_limits<float>::max();
	glm::vec2 MTV = glm::vec2(0, 0);

	// Iterate through axes
	for (const glm::vec2& axis : axes) {
		// Project shapes onto axis
		glm::vec2 p1 = b1->project(axis);
		glm::vec2 p2 = b2->project(axis);

		// If they don't overlap, Bodies dont collide
		if (p1.x > p2.y || p1.y < p2.x) {
			// Returns 0 vector
			return glm::vec2(0, 0);
		}
		else {
			// Calculate the overlap distance between the two shadows
			float newOverlap = std::min(p1.y, p2.y) - std::max(p1.x, p2.x);
			if (newOverlap < overlap) {
				// If the overlap is smaller than the current smallest overlap
				// The current smallest overlap becomes newOverlap
				overlap = newOverlap;
				// The Minimum Translation Vector is equal the the normalized axis multiplied by the size of the overlap
				MTV = glm::normalize(axis) * (overlap);
			}
		}
	}

	return MTV;
}

void Simulation::collisionResloution(Body* b1, Body* b2, glm::vec2 MTV) {

	if (b1->getStatic() && b2->getStatic()) {
		// If both are static, do nothing
		return;
	}

	// Position Correction
	positionCorrection(b1, b2, MTV);

	// Impulse Calculation
	impulseCalculation(b1, b2, MTV);
}

void Simulation::positionCorrection(Body* b1, Body* b2, glm::vec2 MTV) {

	// Move objects
	if (b1->getStatic()) {
		// If b1 is static, only move b2
		b2->addPosition(-MTV);
	}
	else if (b2->getStatic()) {
		// If b2 is static, only move b1
		b1->addPosition(MTV);
	}
	else {
		// Otherwise move both objects proportional to their masses
		// Calculate reciprocal masses
		float rMass1 = 1 / b1->getMass();
		float rMass2 = 1 / b2->getMass();

		glm::vec2 moveVector = MTV / (rMass1 + rMass2);

		b1->addPosition(-moveVector * rMass1);
		b2->addPosition(moveVector * rMass2);
	}

}

void Simulation::impulseCalculation(Body* b1, Body* b2, glm::vec2 MTV) {

	// Calculates collision normal
	glm::vec2 normal = glm::normalize(MTV);

	if (b1->getStatic() || b2->getStatic()) {
		// Find which body is static (cant be both as it is checked in collisionResolution)
		Body* body = b1;
		if (b1->getStatic()) {
			body = b2;
		}

		// Calculates the component of body's velocity parallel to the normal 
		glm::vec2 par = normal * (glm::dot(body->getVelocity(), normal) / glm::dot(normal, normal));
		// Calculates the component of body's velocity perpendicular to the normal
		glm::vec2 perp = body->getVelocity() - par;
		
		// Impulse = mass * change in velocity
		// Change in velocity = perp - par (reflects the body in the opposite direction to the normal)
		// Elasticity detemines how much energy is conservec in the reaction
		glm::vec2 impulse = (perp - par) * (elasticity * body->getMass() * 2);
		body->applyImpulse(impulse);
	}
	else {
		// If neither of them are static

		// Find the relative velocity of the objects
		glm::vec2 rv = b2->getVelocity() - b1->getVelocity(); 

		// Find the relative velocity along the normal
		float velAlongNormal = glm::dot(rv, normal);

		// Calculate the impulse scalar
		float j = (1 + elasticity) * velAlongNormal;
		j /= 1 / b1->getMass() + 1 / b2->getMass();

		// Apply the impulse to the objects
		glm::vec2 impulse = normal * j;

		b1->applyImpulse(impulse);
		b2->applyImpulse(-impulse);
	}
}