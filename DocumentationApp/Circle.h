#pragma once

#include "Rigidbody.h"

class Circle : public Rigidbody {

public:

	Circle();
	Circle(double rad, glm::vec2 position, glm::vec2 velocity, glm::vec2 Force, double Mass, ImVec4 col, bool stat);

	void render(Renderer* renderer) override;


protected:

	double radius;

};