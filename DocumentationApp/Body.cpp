#include "Body.h"
#include "Utilities.h"
#include "Renderer.h"
#include <iostream>

Body::Body(glm::vec2 position, glm::vec2 velocity, glm::vec2 force, float mass, ImVec4 colour, bool isStatic) :
	position(position), velocity(velocity), force(force), mass(mass), colour(colour), isStatic(isStatic) {

}

Body::Body() {

}

void Body::update(double dt) {

}

void Body::render(Renderer* renderer) {
	renderer->drawCircle(position, 1, colour);
}

std::string Body::save() {
	std::ostringstream os;
	os << "body ";
	os << utilities::vec_to_str(position) << " ";
	os << utilities::vec_to_str(velocity) << " ";
	os << utilities::vec_to_str(force) << " ";
	os << utilities::to_str(mass) << " ";
	os << utilities::imvec_to_str(colour) << " ";
	os << utilities::to_str(isStatic);
	return os.str();
}

Body Body::loadBody(std::vector<std::string> data) {

	glm::vec2 pos = utilities::str_to_vec(data[1]);
	glm::vec2 vel = utilities::str_to_vec(data[2]);
	glm::vec2 frc = utilities::str_to_vec(data[3]);
	float mss = utilities::str_to_float(data[4]);
	ImVec4 col = utilities::str_to_imvec(data[5]);
	bool stat = utilities::str_to_bool(data[6]);

	return Body(pos, vel, frc, mss, col, stat);

}

void Body::applyImpulse(glm::vec2 impulse) {
	this->impulse += impulse;
}

glm::vec2 Body::getPosition() {
	return position;
}

glm::vec2 Body::getForce() {
	return force;
}

glm::vec2 Body::getVelocity() {
	return velocity;
}

float Body::getMass() {
	return mass;
}

bool Body::getStatic() {
	return isStatic;
}

ImVec4 Body::getColour() {
	return colour;
}

std::vector<Body*> Body::getCollisionBodies() {
	return std::vector<Body*>();
}

glm::vec2 Body::project(glm::vec2 axis) {
	return glm::vec2();
}

std::vector<glm::vec2> Body::getAxes() {
	return std::vector<glm::vec2>();
}

void Body::addPosition(glm::vec2 pos) {
	position += pos;
}

void Body::setPosition(glm::vec2 pos) {
	position = pos;
}

glm::vec2* Body::getPositionP() {
	return &position;
}

glm::vec2* Body::getVelocityP() {
	return &velocity;
}

glm::vec2* Body::getForceP() {
	return &force;
}

float* Body::getMassP() {
	return &mass;
}

bool* Body::getStaticP() {
	return &isStatic;
}

ImVec4* Body::getColourP() {
	return &colour;
}