#pragma once

#include "glm/glm.hpp"
#include "Collisions.h"
#include <memory>
#include <vector>

class Renderer;

class Body {

public:

	virtual void update(double dt);
	virtual std::vector<Body*> getCollisionBodies();
	virtual void render(Renderer* renderer);
	virtual Projection project(glm::vec2 axis);
	virtual std::vector<glm::vec2> getAxes();

};