#pragma once

#include "glm/vec2.hpp"
#include <memory>
#include <vector>

#include "Body.h"
#include "Rigidbody.h"
#include "Rectangle.h"
#include "Square.h"
#include "Circle.h"
#include "Softbody.h"


#ifndef PBody
#define PBody std::shared_ptr<Body>
#endif // !PBody


class Simulation {

public:

	Simulation();
	~Simulation();

	void update(double dt);

	glm::vec2 getSimulationSize();
	void setSimulationSize(glm::vec2 size);

	std::vector<PBody> getBodies();

	PBody getClickedObject(glm::vec2 pos);

	// Saving/Loading
	void save(std::string path); 
	void load(std::string path);

	void parseLine(std::string);

	// Add Objects
	void addBody(const Body& body);
	void addRigidbody(const Rigidbody& body);
	void addRectangle(const Rectangle& body);
	void addSquare(const Square& body);
	void addCircle(const Circle& body);
	void addSoftbody(const Softbody& body);

	void pausePlay();
	void pause();
	void play();
	void clearSimulation();

	bool* getCollisionsP();
	float* getElasticityP();
	float* getSpeed();

private:

	glm::vec2 simSize;
	std::vector<PBody> bodies;
	bool playing;

	float speed = 1;

	bool collisions;
	float elasticity = 1.0f;

	void collisionDetection();
	glm::vec2 checkCollision(Body* b1, Body* b2); 
	void collisionResloution(Body* b1, Body* b2, glm::vec2 MTV);
	void positionCorrection(Body* b1, Body* b2, glm::vec2 MTV);
	void impulseCalculation(Body* b1, Body* b2, glm::vec2 MTV);

};
