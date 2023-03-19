#include "Simulation.h"
#include "Renderer.h"
#include <algorithm>

Simulation::Simulation() {
	bodies = std::vector<PBody>();


	glm::vec2 pos = glm::vec2(300, 150);
	glm::vec2 pos1 = glm::vec2(100, 200);
	glm::vec2 pos2 = glm::vec2(800, 300);
	glm::vec2 pos3 = glm::vec2(500, 400);
	glm::vec2 vel = glm::vec2(100, 100);
	glm::vec2 vel2 = glm::vec2(0, 100);

	Circle c2 = Circle(20, pos3, vel2, glm::vec2(0, 0), 1, ImVec4(0.0f, 1.0f, 1.0f, 1.0f), false);
	Circle c1 = Circle(20, pos2, glm::vec2(-200, 0), glm::vec2(0, 0), 1, ImVec4(0.0f, 1.0f, 1.0f, 1.0f), false);
	Square s2 = Square(20, pos1, glm::vec2(0, 100), glm::vec2(0, 0), 1, ImVec4(1.0f, 1.0f, 0.0f, 1.0f), false);
	Square s1 = Square(20, pos, vel, glm::vec2(50, 0), 1, ImVec4(1.0f, 1.0f, 0.0f, 1.0f), false);

	s1.setAngle(3.141 / 4);

	bodies.push_back(std::make_shared<Square>(s1));
	//bodies.push_back(std::make_shared<Square>(s2));
	bodies.push_back(std::make_shared<Circle>(c1));
	//bodies.push_back(std::make_shared<Circle>(c2));

	Circle c3 = Circle(20, glm::vec2(500, 500), glm::vec2(0, 0), glm::vec2(0, 0), 1, ImVec4(0.4, 0.5, 0.245, 1.0), false);
	//bodies.push_back(std::make_shared<Circle>(c3));

	// SPRING TEST
	glm::vec2 circlepos = glm::vec2(200, 200);
	glm::vec2 dif = glm::vec2(100, 0);
	Circle cs1 = Circle(10, circlepos, glm::vec2(0, 0), glm::vec2(0, 0), 1, ImVec4(1.0f, 0.0f, 0.0f, 1.0f), false);
	Circle cs2 = Circle(10, circlepos+dif, glm::vec2(0, 0), glm::vec2(0, 0), 1, ImVec4(1.0f, 1.0f, 0.0f, 1.0f), false);

	std::shared_ptr<Circle> cs1p = std::make_shared<Circle>(cs1);
	std::shared_ptr<Circle> cs2p = std::make_shared<Circle>(cs2);

	//bodies.push_back(cs1p);
	//bodies.push_back(cs2p);

	//spring = Spring(cs2p, cs1p, 50, 3);

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

	// Spring TEST:
	//spring.update(dt);
	
	traceBodies.erase(std::remove_if(traceBodies.begin(), traceBodies.end(), [](const Tracer& i) {return i.getDestroy(); }), traceBodies.end());

	for (int i = 0; i < bodies.size(); i++) {

		bodies[i]->update(dt);

		if (tracing)
			traceBodies.push_back(Tracer(bodies[i]->getPostition(), 20, bodies[i]->getColour()));

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
	std::vector<glm::vec2> axes = b1->getAxes();
	std::vector<glm::vec2> b2axes = b2->getAxes();
	axes.insert(std::end(axes), std::begin(b2axes), std::end(b2axes));

	float overlap = -std::numeric_limits<float>::max();
	glm::vec2 MTV = glm::vec2(0, 0);

	// Iterate through axes
	for (const glm::vec2& axis : axes) {
		// Project shapes onto axis
		Projection p1 = b1->project(axis);
		Projection p2 = b2->project(axis);

		// If they don't overlap, Bodies dont collide
		// (min > proj2.max || max < proj2.min);
		float distance1 = (p2.max - p1.min);
		float distance2 = (p1.max - p2.min);
			
		//if (p1.seperate(p2)){
		std::cout << "Distance1: " << distance1 << std::endl;
		std::cout << "Distance2: " << distance2 << std::endl;
		std::cout << "Axis: " << axis.x << " " << axis.y << std::endl;
		if (distance1 < 0 || distance2 < 0){
			return glm::vec2(0, 0);
		}
		else {
			float distance = std::min(distance1, distance2);
			if (distance > overlap) {
				overlap = distance;
				MTV = glm::normalize(axis) * glm::fvec1(overlap);
			}
		}
	}
	return MTV;


}

void Simulation::resolveCollision(Body* b1, Body* b2, float elasticity, glm::vec2 MTV) {

	// POSITION CORRECTION

	// TODO: POSITION CORRECTION

	double rMass1 = 1 / b1->getMass();
	double rMass2 = 1 / b2->getMass();

	glm::vec2 correction = MTV * glm::fvec1(1/ (rMass1 + rMass2));
	std::cout << "Correction: " << correction.x << " " << correction.y << std::endl;
	std::cout << "MTV: " << MTV.x << " " << MTV.y << std::endl;
	std::cout << "Scale: " << 1 / (rMass1 + rMass2) << std::endl;
	b1->addPosition(correction*glm::fvec1(-rMass1));
	b2->addPosition(correction*glm::fvec1(rMass2));

	// APPLY IMPULSES TO OBJECTS:
	
	if (b1->getStatic() && b2->getStatic()) {
		return;
	}
	else if (b1->getStatic()) {
		glm::vec2 normal = b2->getPostition() - b1->getPostition();
		normal = glm::normalize(normal);

		glm::vec2 par = normal * (glm::dot<float>(b2->getVelocity(), normal)/glm::dot(normal, normal);
		glm::vec2 perp = b2->getVelocity()-par;
		glm::vec2 impulse = glm::fvec1(elasticity * b2->getMass() * 2) * (par - perp);
		b2->applyImpulse(impulse);

	}
	else if (b2->getStatic()) {
		glm::vec2 normal = b2->getPostition() - b1->getPostition();
		normal = glm::normalize(normal);

		glm::vec2 par = normal * glm::fvec1(glm::dot<float>(b1->getVelocity(), normal) / glm::dot(normal, normal));
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