#pragma once

#include <vector>
#include "glm/vec2.hpp"
#include <memory>

#include "Body.h"
#include "Circle.h"
#include "Square.h"

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

private:

	glm::vec2 simSize; // Size of the simulation
	std::vector<PBody> bodies;

	void CollisionDetection();
	bool checkCollision(Body* b1, Body* p2);
	void resolveCollision(Body* b1, Body* p2);

};

