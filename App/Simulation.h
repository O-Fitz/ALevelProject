#pragma once

#include <vector>
#include "Body.h"

class Simulation{

private:

	Vector simSize; // Size of the simulation

public:

	Simulation();
	~Simulation();

	void update();

	Vector getSimulationSize();
	void setSimulationSize(Vector newSize);

	std::vector<Body> getBodies();

private:



};

