#pragma once

#include "Rigidbody.h"

class Circle : public Rigidbody{

private:

	float radius;
	int collisionAxis = 15;

public:

	Circle();
	Circle(float rad, glm::vec2 position, glm::vec2 velocity, glm::vec2 Force, float Mass, ImVec4 col, bool stat);

	void render(Renderer* renderer) override;
	Projection project(glm::vec2 axis) override;
	std::vector<glm::vec2> getAxes() override;
	float getRadius();


	void print();
};