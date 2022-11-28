#pragma once

#include "glm/glm.hpp"
#include "Collisions.h"

class Renderer;

class Body {

public:

	void render(Renderer* renderer);
	Projection project(glm::vec2 axis);
	void update(double dt);

};