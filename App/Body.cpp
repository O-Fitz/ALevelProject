#include "Body.h"
#include "Renderer.h"
#include "Utilities.h"

Body::Body(glm::vec2 position, glm::vec2 velocity, glm::vec2 force, float mass, ImVec4 colour, bool isStatic) :
	position(position), velocity(velocity), force(force), mass(mass), colour(colour), isStatic(isStatic) {

}

Body::Body() :
	position(glm::vec2()), velocity(glm::vec2()), force(glm::vec2()), mass(1.0), colour(ImVec4()), isStatic(false) {

}

void Body::update(double dt) {

}

std::string Body::save() {
	std::ostringstream os;
	os << "body ";
	os << utilities::vec_to_str(position) << " ";
	os << utilities::vec_to_str(velocity) << " ";
	os << utilities::vec_to_str(force) << " ";
	os << utilities::to_str(mass) << " ";
	os << utilities::imvec_to_str(colour) << " ";
	os << utilities::to_str(isStatic) << " ";
	return os.str();
}

void Body::render(Renderer* renderer) {
	std::cout << "BODY RENDER" << std::endl;
}

Projection Body::project(glm::vec2 axis) {
	return Projection(axis);
}

std::vector<glm::vec2> Body::getAxes() {
	return std::vector<glm::vec2>();
}

std::vector<Body*> Body::getCollisionBodies() {
	return std::vector<Body*>({this});
}

void Body::applyImpulse(glm::vec2 impulse) {
	this->impulse += impulse;
}

glm::vec2 Body::getPostition() {
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

void Body::setAngle(double ang) {
	angle = ang;
}

double Body::getAngle() {
	return angle;
}

void Body::addPosition(glm::vec2 pos) {
	position += pos;
}