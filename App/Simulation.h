#pragma once

#include <vector>
#include "glm/vec2.hpp"
#include <memory>

#include "Body.h"
#include "Circle.h"
#include "Square.h"
#include "Spring.h"

#include "Tracer.h"

#ifndef PBody 
#define PBody std::shared_ptr<Body>
#endif

class Simulation{
public:	

	Simulation();
	~Simulation();

	void update(double dt);

	void save(std::string path);
	void load(std::string path);

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
	glm::vec2 getGravPoint();
	glm::vec2* getGravPointP();

	void pausePlay();

private:

	glm::vec2 simSize; // Size of the simulation
	std::vector<PBody> bodies;
	std::vector<Tracer> traceBodies;
	bool running;
	float elasticity=0.2f;
	bool collisions = true;
	bool gravity = false;
	glm::vec2 gravPoint = glm::vec2(400, 400);
	double gravStrength = 1000000;
	bool tracing = true;

	// Spring Test
	Spring spring;

	void CollisionDetection();
	glm::vec2 checkCollision(Body* b1, Body* p2);
	void resolveCollision(Body* b1, Body* p2, float elasticity, glm::vec2 MTV);

	void parseSaveLine(std::string line);

};

