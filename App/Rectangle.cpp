#include "Rectangle.h"

Rectangle::Rectangle() : Rigidbody() {

}

Rectangle::Rectangle(double width, double height) : 
	Rigidbody(sizeToVerticies(width, height)), mWidth(width), mHeight(height){

}


Rectangle::Rectangle(double width, double height, glm::vec2 position, glm::vec2 vel, glm::vec2 Force, double Mass, ImVec4 colour, bool stat) :
	Rigidbody(sizeToVerticies(width, height), position, vel, Force, Mass, colour, stat), mWidth(width), mHeight(height) {

}

std::vector<glm::vec2> Rectangle::sizeToVerticies(double width, double height) {
	std::vector<glm::vec2> vert = std::vector<glm::vec2>();
	double h =  height / 2;
	double w =  width / 2;
	vert.push_back(glm::vec2(-w, -h));
	vert.push_back(glm::vec2(w, -h));
	vert.push_back(glm::vec2(w, h));
	vert.push_back(glm::vec2(-w, h));
	return vert;
}