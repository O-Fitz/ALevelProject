#pragma once

#include "Rigidbody.h"

class Rectangle : public Rigidbody {

public:

	Rectangle();
	Rectangle(double width, double height);
	Rectangle(double width, double height, glm::vec2 position, glm::vec2 vel, glm::vec2 Force, double Mass, ImVec4 colour, bool stat);

protected:

	double width;
	double height;

	std::vector<glm::vec2> sizeToVertices(double width, double height);
};