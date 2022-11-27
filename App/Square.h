#pragma once
#include "Rectangle.h"

class Square : public Rectangle {

private:

	double mSideLength;

public:
	Square();
	Square(double sideLength);
	Square(double sideLength, Vector position, Vector vel, Vector Force, double Mass, ImVec4 colour);

private:

};

