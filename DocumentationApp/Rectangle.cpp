#include "Rectangle.h"

Rectangle::Rectangle() : Rigidbody() {

}

Rectangle::Rectangle(double width, double height) :
	Rigidbody(sizeToVertices(width, height)), width(width), height(height) {

}


Rectangle::Rectangle(double width, double height, glm::vec2 position, glm::vec2 vel, glm::vec2 Force, double Mass, ImVec4 colour, bool stat) :
	Rigidbody(sizeToVertices(width, height), position, vel, Force, Mass, colour, stat), width(width), height(height) {

}

std::vector<glm::vec2> Rectangle::sizeToVertices(double width, double height) {
	std::vector<glm::vec2> vert = std::vector<glm::vec2>();
	double h = height / 2;
	double w = width / 2;
	vert.push_back(glm::vec2(-w, -h));
	vert.push_back(glm::vec2(w, -h));
	vert.push_back(glm::vec2(w, h));
	vert.push_back(glm::vec2(-w, h));
	return vert;
}