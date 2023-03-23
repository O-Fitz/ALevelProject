#include "Simulation.h"
#include "Renderer.h"
#include "Softbody.h"
#include <algorithm>

Simulation::Simulation() {
	bodies = std::vector<PBody>();


	glm::vec2 pos = glm::vec2(400, 450);
	glm::vec2 zero = glm::vec2(0, 0);
	ImVec4 red = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	ImVec4 green = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec2 pos1 = glm::vec2(400, 100);
	glm::vec2 pos2 = glm::vec2(400, 300);
	//glm::vec2 pos3 = glm::vec2(500, 400);
	glm::vec2 vel = glm::vec2(0, -50);
	//glm::vec2 vel2 = glm::vec2(0, 100);

	//Circle c2 = Circle(20, pos3, vel2, glm::vec2(0, 0), 1, ImVec4(0.0f, 1.0f, 1.0f, 1.0f), false);
	//Circle c1 = Circle(20, pos2, glm::vec2(-200, 0), glm::vec2(0, 0), 1, ImVec4(0.0f, 1.0f, 1.0f, 1.0f), false);
	//Square s2 = Square(20, pos1, glm::vec2(0, 100), glm::vec2(0, 0), 1, ImVec4(1.0f, 1.0f, 0.0f, 1.0f), false);
	//Square s1 = Square(20, pos, vel, glm::vec2(50, 0), 1, ImVec4(1.0f, 1.0f, 0.0f, 1.0f), false);

	//s1.setAngle(3.141 / 4);

	////bodies.push_back(std::make_shared<Square>(s1));
	////bodies.push_back(std::make_shared<Square>(s2));
	////bodies.push_back(std::make_shared<Circle>(c1));
	////bodies.push_back(std::make_shared<Circle>(c2));

	//Circle c3 = Circle(20, glm::vec2(500, 500), glm::vec2(0, 0), glm::vec2(0, 0), 1, ImVec4(0.4, 0.5, 0.245, 1.0), false);
	////bodies.push_back(std::make_shared<Circle>(c3));


	Softbody soft = Softbody(pos, vel, vel/5.0f, 5, red, false, 10, 50, 50, 0.9);
	bodies.push_back(std::make_shared<Softbody>(soft));

	Rectangle sst = Rectangle(900, 500, pos1, zero, zero, 5, green, true);
	Square sst1 = Square(50, pos, vel, zero, 5, green, false);
	Circle cst = Circle(25, pos2, zero, zero, 5, red, false);
	Circle cst1 = Circle(5, pos+glm::vec2(15, 0), zero, zero, 5, red, false);
	bodies.push_back(std::make_shared<Rectangle>(sst));
	//bodies.push_back(std::make_shared<Circle>(cst));
	//bodies.push_back(std::make_shared<Circle>(cst1));
	//bodies.push_back(std::make_shared<Square>(sst1));

}

Simulation::~Simulation() {

}

void Simulation::update(double dt) {

	//dt *= 2;

	// Exit function if simulation not running
	if (!running) {
		return;
	}

	for (int i = 0; i < traceBodies.size(); i++) {
		traceBodies[i].update(dt);
	}

	
	traceBodies.erase(std::remove_if(traceBodies.begin(), traceBodies.end(), [](const Tracer& i) {return i.getDestroy(); }), traceBodies.end());

	for (int i = 0; i < bodies.size(); i++) {

		bodies[i]->update(dt);

		if (tracing) {}
			//traceBodies.push_back(Tracer(bodies[i]->getPostition(), 20, bodies[i]->getColour()));

	}

	if (gravity) {
		for (int i = 0; i < bodies.size(); i++) {
			glm::vec2 sep = gravPoint - bodies[i]->getPostition();
			glm::fvec1 strength = glm::fvec1(gravStrength*dt*bodies[i]->getMass())/glm::dot(sep, sep); // GMm/r^2
			bodies[i]->applyImpulse(glm::normalize(sep) * strength);
		}
	}

	if (collisions)
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
			if (!checked[j]) {
				glm::vec2 MTV = checkCollision(collisionBodies[i], collisionBodies[j]);
				if (MTV != glm::vec2(0, 0)) {
					resolveCollision(collisionBodies[i], collisionBodies[j], elasticity, MTV);
				}
			}
		}
	}

}

glm::vec2 Simulation::checkCollision(Body* b1, Body* b2) {

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
	// Find all of the axes and append them to one vector
	std::vector<glm::vec2> axes = b1->getAxes();
	std::vector<glm::vec2> b2axes = b2->getAxes();
	axes.insert(std::end(axes), std::begin(b2axes), std::end(b2axes));

	float overlap = std::numeric_limits<float>::max();
	glm::vec2 MTV = glm::vec2(0, 0);

	// Iterate through axes
	for (const glm::vec2& axis : axes) {
		// Project shapes onto axis
		Projection p1 = b1->project(axis);
		Projection p2 = b2->project(axis);

		// If they don't overlap, Bodies dont collide
		if (p1.min > p2.max || p1.max < p2.min) {
			// Returns 0 vector
			return glm::vec2(0, 0);
		}
		else {
			// Calculate the overlap distance between the two shadows
			float newOverlap = std::min(p1.max, p2.max) - std::max(p1.min, p2.min);
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

void Simulation::resolveCollision(Body* b1, Body* b2, float elasticity, glm::vec2 MTV) {

	if (b1->getStatic() && b2->getStatic()) {
		return;
	}
	// POSITION CORRECTION

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

std::vector<Tracer> Simulation::getTraceBodies() {
	return traceBodies;
}

bool Simulation::getCollisions() {
	return collisions;
}

bool* Simulation::getCollisionsP() {
	return &collisions;
}

bool Simulation::getGravity() {
	return gravity;
}

bool* Simulation::getGravityP() {
	return &gravity;
}

float Simulation::getElasticity() {
	return elasticity;
}

float* Simulation::getElasticityP() {
	return &elasticity;
}

glm::vec2 Simulation::getGravPoint() {
	return gravPoint;
}

glm::vec2* Simulation::getGravPointP() {
	return &gravPoint;
}