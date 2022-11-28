#pragma once

#include "Rigidbody.h"

class Circle : public Rigidbody{

private:

	double radius;

public:

	Circle();
	Circle(double rad, glm::vec2 position, glm::vec2 velocity, glm::vec2 Force, double Mass, ImVec4 col);

	void render(Renderer* renderer);
	Projection project(glm::vec2 axis);

	void print();
};