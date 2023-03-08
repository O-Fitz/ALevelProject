#include "Body.h"

Body::Body(glm::vec2 position, glm::vec2 velocity, glm::vec2 force, float mass, ImVec4 colour, bool isStatic) :
	position(position), velocity(velocity), force(force), mass(mass), colour(colour), isStatic(isStatic) {

}

Body::Body() {

}

void Body::update(double dt) {

}

void Body::render(Renderer* renderer) {

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

double Body::getMass() {
	return mass;
}

bool Body::getStatic() {
	return isStatic;
}

ImVec4 Body::getColour() {
	return colour;
}