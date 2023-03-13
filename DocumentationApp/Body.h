#pragma once

#include "glm/glm.hpp"
#include "imguiInclude.h"

#include <vector>

class Renderer;

class Body {
public:
	Body();
	Body(glm::vec2 position, glm::vec2 velocity, glm::vec2 force, float mass, ImVec4 colour, bool isStatic);

	virtual void update(double dt);
	virtual void render(Renderer* renderer);

	glm::vec2 getPostition();
	glm::vec2 getVelocity();
	glm::vec2 getForce();
	double getMass();
	bool getStatic();
	ImVec4 getColour();

	void applyImpulse(glm::vec2 impulse);

	virtual std::vector<Body*> getCollisionBodies();
	virtual glm::vec2 project(glm::vec2 axis);
	virtual std::vector<glm::vec2> getAxes();

protected:
	glm::vec2 impulse;
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 force;
	float mass;
	ImVec4 colour;
	bool isStatic;
};