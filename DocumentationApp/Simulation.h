#pragma once

#include "glm/vec2.hpp"
#include <memory>
#include <vector>

#include "Body.h"

class Simulation {

public:

	Simulation();
	~Simulation();

	void update(double dt);

	glm::vec2 getSimulationSize();
	void setSimulationSize(glm::vec2 size);

	std::vector<Body> getBodies();

private:

	glm::vec2 simSize;
	std::vector<Body> bodies;
	bool running;

};