#pragma once

#include "glm/vec2.hpp"
#include <memory>
#include <vector>

#include "Body.h"
#include "Rigidbody.h"

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
	std::vector<PBody> getBodies();

private:

	glm::vec2 simSize;
	std::vector<PBody> bodies;
	bool running;

	float elasticity = 1.0f;

	glm::vec2 checkCollision(Body* b1, Body* b2); 
	void collisionResloution(Body* b1, Body* b2, glm::vec2 MTV);
	void positionCorrection(Body* b1, Body* b2, glm::vec2 MTV);
	void impulseCalculation(Body* b1, Body* b2, glm::vec2 MTV);

};