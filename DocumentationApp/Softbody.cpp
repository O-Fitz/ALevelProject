#include "Softbody.h"
#include "Renderer.h"

Softbody::Softbody(glm::vec2 position, glm::vec2 velocity, glm::vec2 force, float mass, ImVec4 colour, bool isStatic, int noPoints, float radius, float springConstant, float dampeningFactor) :
	Body(position, velocity, force, mass, colour, isStatic), noPoints(noPoints), radius(radius), springConstant(springConstant), dampeningFactor(dampeningFactor) {

	points = std::vector<Point>();
	springs = std::vector<Spring>();

	setupPoints();
	setupSprings();
}

void Softbody::setupPoints() {

	// Iterates through angles around a cirlce
	for (float i = 0; i < 2 * 3.141; i += 2 * 3.141 / noPoints) {
		
		// Position on the circle
		glm::vec2 pos = glm::vec2(cos(i), sin(i)) * radius;
		// Creates a circle at that point with similar properties to the softbody
		Circle c = Circle(5, pos + position, velocity, force, mass / noPoints, colour, isStatic);
		// Make the circle a shared pointer and add it to points
		points.push_back(std::make_shared<Circle>(c));

	}
}

void Softbody::setupSprings() {

	// Stores if spring has already been made for the springs
	std::vector<bool>added = std::vector<bool>();
	for (int i = 0; i < points.size(); i++) {
		added.push_back(false);
	}

	// Iterates through all combinations of points
	for (int i = 0; i < points.size(); i++) {
		added[i] = true;
		for (int j = 0; j < points.size(); j++) {
			if (!added[j]) {
				// Calculates distance
				float distance = glm::length(points[i]->getPosition() - points[j]->getPosition());
				springs.emplace_back(points[i], points[j], distance, springConstant, dampeningFactor);

			}
		}
	}

}

void Softbody::update(double dt) {

	// Return out if static
	if (isStatic) {
		return;
	}

	// Velocity and position update
	velocity += (force / mass) * (float)dt + impulse / mass;
	impulse = glm::vec2(0, 0);
	position += velocity * (float)dt;

	// Update all of the springs
	for (int i = 0; i < springs.size(); i++) {
		springs[i].update(dt);
	}

	// Update all of the points
	for (int i = 0; i < points.size(); i++) {
		points[i]->update(dt);
	}

}

void Softbody::render(Renderer* renderer) {

	// Render Springs
	for (int i = 0; i < springs.size(); i++) {
		springs[i].render(renderer, colour);
	}

	// Render Points
	for (int i = 0; i < points.size(); i++) {
		points[i]->render(renderer);
	}

}