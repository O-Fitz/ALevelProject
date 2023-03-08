#pragma once

#include "Rectangle.h"

class Square : public Rectangle {

public:

	Square();
	Square(double sideLength);
	Square(double sideLength, glm::vec2 position, glm::vec2 vel, glm::vec2 Force, double Mass, ImVec4 colour, bool stat);

protected:

	double sideLength;

};