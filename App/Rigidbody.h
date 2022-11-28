#pragma once
#include "Body.h"
#include "Renderer.h"
#include <vector>

class Rigidbody : public Body {

protected:
	std::vector<Vector> verticies;
	Vector position;
	Vector velocity;
	Vector force;
	double mass;
	ImVec4 colour;

public:

	Rigidbody();
	Rigidbody(std::vector<Vector> vert);
	Rigidbody(std::vector<Vector> vert, Vector pos, Vector vel, Vector Force, double Mass, ImVec4 col);

	void render(Renderer* renderer);
	Projection project(Vector axis);
	
};