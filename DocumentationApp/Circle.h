#pragma once

#include "Rigidbody.h"

class Circle : public Rigidbody {

public:

	Circle();
	Circle(double rad, glm::vec2 position, glm::vec2 velocity, glm::vec2 Force, double Mass, ImVec4 col, bool stat);

	void render(Renderer* renderer) override;

	std::string save() override;
	static Circle loadCircle(std::vector<std::string> data);

	glm::vec2 project(glm::vec2 axis) override;
	std::vector<glm::vec2> getAxes() override;


protected:

	double radius;
	int collisionAxis = 20;
};