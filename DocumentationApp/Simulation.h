#pragma once

#include "glm/vec2.hpp"
#include <memory>
#include <vector>

#include "Body.h"

#ifndef PBody
#define PBody std::shared_ptr<Body>
#endif // !PBody


class Simulation {

public:

	Simulation();
	~Simulation();

	void update(double dt);

	glm::vec2 getSimulationSize();
	void setSimulationSize(glm::vec2 size);

	void collisionDetection();
	bool checkCollision(Body* b1, Body* b2);

	std::vector<PBody> getBodies();

private:

	glm::vec2 simSize;
	std::vector<PBody> bodies;
	bool running;

};