#pragma once
#include "Body.h"
#include <vector>

class Rigidbody : public Body {

private:
	std::vector<Vector> verticies;
	Vector position;
	Vector velocity;
	Vector force;
	double mass;

public:

	Rigidbody();
	Rigidbody(std::vector<Vector> vert, Vector pos, Vector vel, Vector Force, double Mass);

	void render(Renderer* renderer);
	Projection project(Vector axis);
	
};