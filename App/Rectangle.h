#pragma once

#include "Rigidbody.h"

class Rectangle : public Rigidbody{

private:

	double mWidth;
	double mHeight;

public:

	Rectangle();
	Rectangle(double width, double height);
	Rectangle(double width, double height, glm::vec2 position, glm::vec2 vel, glm::vec2 Force, double Mass, ImVec4 colour, bool stat);

	std::string save() override;
	static Rectangle loadRectangle(std::vector<std::string> data);

private:

	std::vector<glm::vec2> sizeToVerticies(double width, double height);

};

