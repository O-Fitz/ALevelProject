#pragma once

#include <vector>
#include "glm/vec2.hpp"
#include <memory>

#include "Body.h"
#include "Circle.h"
#include "Square.h"

#include "Tracer.h"

#ifndef PBody 
#define PBody std::shared_ptr<Body>
#endif

class Simulation{
public:	

	Simulation();
	~Simulation();

	void update(double dt);

	glm::vec2 getSimulationSize();
	void setSimulationSize(glm::vec2 newSize);

	std::vector<PBody> getBodies();
	std::vector<Tracer> getTraceBodies();

	bool getCollisions();
	bool* getCollisionsP();
	bool getGravity();
	bool* getGravityP();
	float getElasticity();
	float* getElasticityP();

	void pausePlay();

private:

	glm::vec2 simSize; // Size of the simulation
	std::vector<PBody> bodies;
	std::vector<Tracer> traceBodies;
	bool running;
	float elasticity=0.2f;
	bool collisions = true;
	bool gravity = true;
	glm::vec2 gravPoint = glm::vec2(400, 400);
	double gravStrength = 100;
	bool tracing = true;

	void CollisionDetection();
	bool checkCollision(Body* b1, Body* p2);
	void resolveCollision(Body* b1, Body* p2, float elasticity);

};

