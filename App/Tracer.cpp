#include "Tracer.h"
#include "Renderer.h"

Tracer::Tracer() {
	position = glm::vec2(0, 0);
}

Tracer::Tracer(glm::vec2 position, double timeLeft, ImVec4 colour) : position(position), timeLeft(timeLeft), colour(colour) {

}

void Tracer::update(double dt) {
	timeLeft -= dt;
}

bool Tracer::getDestroy() const{
	return timeLeft <= 0;
}

void Tracer::render(Renderer* renderer) {
	renderer->drawCircle(position, 1, colour);
}