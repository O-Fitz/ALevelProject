#pragma once

#include "Rigidbody.h"

class Rectangle : public Rigidbody{

private:

	double mWidth;
	double mHeight;

public:

	Rectangle();
	Rectangle(double width, double height);
	Rectangle(double width, double height, Vector position, Vector vel, Vector Force, double Mass, ImVec4 colour);

private:

	std::vector<Vector> sizeToVerticies(double width, double height);

};

