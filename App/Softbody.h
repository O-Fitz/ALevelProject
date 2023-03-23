#pragma once

#include "Body.h"
#include "Spring.h"
#include "Circle.h"

#ifndef Point
#define Point std::shared_ptr<Circle>
#endif // Point

class Softbody : public Body {

public:

	Softbody(glm::vec2 position, glm::vec2 velocity, glm::vec2 force, float mass, ImVec4 colour, bool isStatic, int noPoints, float radius, float springConstant, float dampeningFactor);

	void update(double dt) override;
	void render(Renderer* renderer) override;

	std::vector<Body*> getCollisionBodies() override;

private:

	std::vector<Point> points;
	std::vector<Spring> springs;

	int noPoints;
	float radius;

	float springConstant;
	float dampeningFactor;

	void setupPoints();
	void setupSprings();

};