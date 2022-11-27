#pragma once

#include "Maths/Vector.h"
#include "Collisions.h"

class Renderer;

class Body {

public:

	void render(Renderer* renderer);
	Projection project(Vector axis);

};