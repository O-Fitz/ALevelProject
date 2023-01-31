#include "Simulation.h"
#include "Renderer.h"

Simulation::Simulation() {
	bodies = std::vector<PBody>();

	glm::vec2 pos = glm::vec2(100, 100);
	glm::vec2 pos1 = glm::vec2(200, 200);
	glm::vec2 pos2 = glm::vec2(300, 300);
	glm::vec2 vel = glm::vec2(100, 100);

	Circle c1 = Circle(10, pos2, glm::vec2(0, 0), glm::vec2(0, 0), 1, ImVec4(0.0f, 1.0f, 1.0f, 1.0f), false);
	Square s2 = Square(20, pos1, glm::vec2(0, 0), glm::vec2(0, 0), 1, ImVec4(1.0f, 1.0f, 0.0f, 1.0f), false);
	Square s1 = Square(20, pos, vel, glm::vec2(0, 0), 1, ImVec4(1.0f, 1.0f, 0.0f, 1.0f), false);

	bodies.push_back(std::make_shared<Circle>(c1));
	bodies.push_back(std::make_shared<Square>(s1));
	bodies.push_back(std::make_shared<Square>(s2));


}

Simulation::~Simulation() {

}

void Simulation::update(double dt) {

	// Exit function if simulation not running
	if (!running) {
		return;
	}

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

void Simulation::pausePlay() {
	running = !running;
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
			float min;
			if (!checked[j] && checkCollision(collisionBodies[i], collisionBodies[j])) {
				resolveCollision(collisionBodies[i], collisionBodies[j], elasticity);
			}
		}
	}

}

bool Simulation::checkCollision(Body* b1, Body* b2) {

	/*
	
	ERROR: 
	FOUND: No collisions were being detected
	DESCRIPTION: The Rigidbody projection wasn't projecting properly, dot product producing unexpected value
	REASONS:
		1. The axis produced was incorrect
		2. Passing in vertex relative position into dot product not actual position
	SOLUTION: 
		1. Rewrote the axis finder
		2. Changed parameter passed in to be position+relativeVertexPosition to find the vertexPosition
	STATUS: Resloved
	
	*/


	// Find all of the axes and append them to one vector
	std::vector<glm::vec2> axes = b1->getAxes();
	std::vector<glm::vec2> b2axes = b2->getAxes();
	axes.insert(std::end(axes), std::begin(b2axes), std::end(b2axes));


	// Iterate through axes
	for (const glm::vec2& axis : axes) {
		// Project shapes onto axis
		Projection p1 = b1->project(axis);
		Projection p2 = b2->project(axis);

		// If they don't overlap, Bodies dont collide
		if (p1.seperate(p2)) {
			return false;
		}
	}

	return true;

}

void Simulation::resolveCollision(Body* b1, Body* b2, float elasticity) {

	// POSITION CORRECTION

	// TODO: POSITION CORRECTION


	// APPLY IMPULSES TO OBJECTS:
	
	if (b1->getStatic() && b2->getStatic()) {
		return;
	}
	else if (b1->getStatic()) {
		glm::vec2 normal = b2->getPostition() - b1->getPostition();
		normal = glm::normalize(normal);


		glm::vec2 par = normal * glm::fvec1(glm::dot<float>(b2->getVelocity(), normal)/glm::dot(normal, normal));
		glm::vec2 perp = b2->getVelocity()-par;
		glm::vec2 impulse = glm::fvec1(elasticity * b2->getMass() * 2) * (par - perp);
		b2->applyImpulse(impulse);

	}
	else if (b2->getStatic()) {
		glm::vec2 normal = b2->getPostition() - b1->getPostition();
		normal = glm::normalize(normal);

		glm::vec2 par = normal * glm::fvec1(glm::dot<float>(b2->getVelocity(), normal) / glm::dot(normal, normal));
		glm::vec2 perp = b1->getVelocity() - par;
		glm::vec2 impulse = glm::fvec1(elasticity*b1->getMass()*2) * (par - perp);
		b1->applyImpulse(impulse);

	}
	else {
		// Find the collision normal
		glm::vec2 normal = b2->getPostition() - b1->getPostition();
		normal = glm::normalize(normal);

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