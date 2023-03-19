#pragma once

#include "glm/glm.hpp"
#include "Collisions.h"
#include "imguiInclude.h"
#include <memory>
#include <vector>

class Renderer;

class Body {

public:

	Body(glm::vec2 position, glm::vec2 velocity, glm::vec2 force, float mass, ImVec4 colour, bool isStatic);
	Body();

	virtual void update(double dt);
	virtual void render(Renderer* renderer);

	virtual Projection project(glm::vec2 axis);
	virtual std::vector<glm::vec2> getAxes();

	virtual void applyImpulse(glm::vec2 impulse);

	glm::vec2 getPostition();
	glm::vec2 getVelocity();
	glm::vec2 getForce();
	double getMass();
	bool getStatic();
	ImVec4 getColour();

	void addPosition(glm::vec2 pos);

	void setAngle(double ang);
	double getAngle();

	virtual std::vector<Body*> getCollisionBodies();

protected:
	glm::vec2 impulse;
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 force;
	float mass;

	double angle;
	double angularVelocity;
	double angularImpulse;

	ImVec4 colour;
	bool isStatic;
};