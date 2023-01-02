#pragma once

#include "glm/glm.hpp"
#include "Collisions.h"

class Renderer;

class Body {

public:

	virtual void update(double dt);
	virtual void render(Renderer* renderer);
	virtual Projection project(glm::vec2 axis);

};