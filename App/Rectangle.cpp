#include "Rectangle.h"

Rectangle::Rectangle() : Rigidbody() {

}

Rectangle::Rectangle(double width, double height) : 
	Rigidbody(sizeToVerticies(width, height)), mWidth(width), mHeight(height){

}


Rectangle::Rectangle(double width, double height, Vector position, Vector vel, Vector Force, double Mass, ImVec4 colour) : 
	Rigidbody(sizeToVerticies(width, height), position, vel, Force, Mass, colour), mWidth(width), mHeight(height) {

}

std::vector<Vector> Rectangle::sizeToVerticies(double width, double height) {
	std::vector<Vector> vert = std::vector<Vector>();
	double h =  height / 2;
	double w =  width / 2;
	vert.push_back(Vector(-w, -h));
	vert.push_back(Vector(w, -h));
	vert.push_back(Vector(w, h));
	vert.push_back(Vector(-w, h));
	return vert;
}