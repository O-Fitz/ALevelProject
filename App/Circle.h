#pragma once

#include "Rigidbody.h"

class Circle : public Rigidbody{

private:

	double radius;
	int collisionAxis = 10;

public:

	Circle();
	Circle(double rad, glm::vec2 position, glm::vec2 velocity, glm::vec2 Force, double Mass, ImVec4 col, bool stat);

	void render(Renderer* renderer) override;
	Projection project(glm::vec2 axis) override;
	std::vector<glm::vec2> getAxes() override;

	void print();
};