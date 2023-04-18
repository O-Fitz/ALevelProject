#include "Simulation.h"

#include "Renderer.h"

#include <sstream>
#include <fstream>

Simulation::Simulation(){
	bodies = std::vector<PBody>();

	playing = false;
	collisions = true;

}

Simulation::~Simulation() {

}

void Simulation::update(double dt) {

	dt *= speed;

	// If simulation is not running
	if (!playing) {
		// Return out of update function before anything else can be executed
		return;
	}

	// Call update functions
	for (int i = 0; i < bodies.size(); i++) {
		bodies[i]->update(dt);
	}

	// Collision Detection
	if (collisions) {
		collisionDetection();
	}
}

void Simulation::save(std::string path) {
	std::ostringstream os;

	for (int i = 0; i < bodies.size(); i++) {
		os << bodies[i]->save();
		if (i != bodies.size() - 1) {
			os << "\n";
		}
	}

	std::ofstream file(path);

	if (file.is_open()) {
		file << os.str();
	}
	file.close();

}

void Simulation::load(std::string path) {
	// Clear current bodies
	bodies = std::vector<PBody>();

	// Open file
	std::ifstream file(path);
	std::string line;

	// Iterated through lines of the file
	while (std::getline(file, line)) {
		// Parse each line
		parseLine(line);
	}

	// Close the file
	file.close();
}

void Simulation::parseLine(std::string line) {

	// Create list of data
	std::vector<std::string> data = std::vector<std::string>();
	// Current data represents string 
	std::string currentData = std::string();

	// Iterate through each character in the string
	for (int i = 0; i < line.size(); i++) {
		// If character is a space or the last character
		if (line[i] == ' ') {
			// Add the current data to the end of the list
			data.push_back(currentData);
			// Clear the current data
			currentData = "";
		}
		else {
			// Otherwise, add the current character to currentData
			currentData.push_back(line[i]);
		}
	}
	data.push_back(currentData);

	// Add bodies
	if (data[0] == "body") {
		bodies.push_back(std::make_shared<Body>(Body::loadBody(data)));
	}
	else if (data[0] == "rigidbody"){
		bodies.push_back(std::make_shared<Rigidbody>(Rigidbody::loadRigidbody(data)));
	}
	else if (data[0] == "rectangle") {
		bodies.push_back(std::make_shared<Rectangle>(Rectangle::loadRectangle(data)));
	}
	else if (data[0] == "square") {
		bodies.push_back(std::make_shared<Square>(Square::loadSquare(data)));
	}
	else if (data[0] == "circle") {
		bodies.push_back(std::make_shared<Circle>(Circle::loadCircle(data)));
	}
	else if (data[0] == "softbody") {
		bodies.push_back(std::make_shared<Softbody>(Softbody::loadSoftbody(data)));
	}
}

std::vector<PBody> Simulation::getBodies() {
	return bodies;
}

glm::vec2 Simulation::getSimulationSize() {
	return simSize;
}

void Simulation::setSimulationSize(glm::vec2 size) {
	simSize = size;
}

void Simulation::collisionDetection() {
	// Find all of the Bodies that can collide
	std::vector<Body*> collisionBodies = std::vector<Body*>();
	for (int i = 0; i < bodies.size(); i++) {
		std::vector<Body*> col = bodies[i]->getCollisionBodies();
		collisionBodies.insert(collisionBodies.end(), col.begin(), col.end());
	}

	// Check each body against every other body
	// checked is the make sure that we dont compare 2 objects twice
	std::vector<bool>checked = std::vector<bool>();
	for (int i = 0; i < collisionBodies.size(); i++) {
		checked.push_back(false);
	}

	for (int i = 0; i < collisionBodies.size(); i++) {
		checked[i] = true;
		for (int j = 0; j < collisionBodies.size(); j++) {
			float min;
			if (!checked[j]) {
				glm::vec2 MTV = checkCollision(collisionBodies[i], collisionBodies[j]);
				if (MTV != glm::vec2(0, 0)) {
					collisionResloution(collisionBodies[i], collisionBodies[j], MTV);
				}
			}
		}
	}
}

glm::vec2 Simulation::checkCollision(Body* b1, Body* b2) {
	// Find all of the axes and append them to one vector
	std::vector<glm::vec2> axes = b1->getAxes();
	std::vector<glm::vec2> b2axes = b2->getAxes();
	axes.insert(std::end(axes), std::begin(b2axes), std::end(b2axes));

	float overlap = std::numeric_limits<float>::max();
	glm::vec2 MTV = glm::vec2(0, 0);

	// Iterate through axes
	for (const glm::vec2& axis : axes) {
		// Project shapes onto axis
		glm::vec2 p1 = b1->project(axis);
		glm::vec2 p2 = b2->project(axis);

		// If they don't overlap, Bodies dont collide
		if (p1.x > p2.y || p1.y < p2.x) {
			// Returns 0 vector
			return glm::vec2(0, 0);
		}
		else {
			// Calculate the overlap distance between the two shadows
			float current_overlap1 = p1.y - p2.x;
			float current_overlap2 = p2.y - p1.x;
			if (current_overlap1 < overlap) {
				// If the current_overlap is smaller than the overlap
				// The overlap becomes current_overlap
				overlap = current_overlap1;
				// The Minimum Translation Vector is equal the the normalized axis multiplied by the size of the overlap
				MTV = glm::normalize(axis) * -overlap;
			}if (current_overlap2 < overlap) {
				// If the current_overlap is smaller than the overlap
				// The overlap becomes current_overlap
				overlap = current_overlap2;
				// The Minimum Translation Vector is equal the the normalized axis multiplied by the size of the overlap
				MTV = glm::normalize(axis) * overlap;
			}
		}
	}
	return MTV;
}

void Simulation::collisionResloution(Body* b1, Body* b2, glm::vec2 MTV) {

	if (b1->getStatic() && b2->getStatic()) {
		// If both are static, do nothing
		return;
	}

	// Position Correction
	positionCorrection(b1, b2, MTV);

	// Impulse Calculation
	impulseCalculation(b1, b2, MTV);
}

void Simulation::positionCorrection(Body* b1, Body* b2, glm::vec2 MTV) {

	// Move objects
	if (b1->getStatic()) {
		// If b1 is static, only move b2
		b2->addPosition(-MTV);
	}
	else if (b2->getStatic()) {
		// If b2 is static, only move b1
		b1->addPosition(MTV);
	}
	else {
		// Otherwise move both objects proportional to their masses
		// Calculate reciprocal masses
		float rMass1 = 1 / b1->getMass();
		float rMass2 = 1 / b2->getMass();

		glm::vec2 moveVector = MTV / (rMass1 + rMass2);

		b1->addPosition(moveVector * rMass1);
		b2->addPosition(-moveVector * rMass2);
	}

}

void Simulation::impulseCalculation(Body* b1, Body* b2, glm::vec2 MTV) {

	// Calculates collision normal
	glm::vec2 normal = glm::normalize(MTV);

	if (b1->getStatic() || b2->getStatic()) {
		// Find which body is static (cant be both as it is checked in collisionResolution)
		Body* body = b1;
		if (b1->getStatic()) {
			body = b2;
		}

		// Calculates the component of body's velocity parallel to the normal 
		glm::vec2 par = normal * (glm::dot(body->getVelocity(), normal) / glm::dot(normal, normal));
		
		// Impulse = mass * change in velocity
		// Change in velocity = - 2 * par (reflects the body in the opposite direction to the normal)
		// Elasticity detemines how much energy is conservec in the reaction
		glm::vec2 impulse = -2.0f * par * elasticity * body->getMass();
		body->applyImpulse(impulse);
	}
	else {
		// If neither of them are static

		// Find the relative velocity of the objects
		glm::vec2 rv = b2->getVelocity() - b1->getVelocity(); 

		// Find the relative velocity along the normal
		float velAlongNormal = glm::dot(rv, normal);

		// Calculate the impulse scalar
		float j = (1 + elasticity) * velAlongNormal;
		j /= 1 / b1->getMass() + 1 / b2->getMass();

		// Apply the impulse to the objects
		glm::vec2 impulse = normal * j;

		b1->applyImpulse(impulse);
		b2->applyImpulse(-impulse);
	}
}

PBody Simulation::getClickedObject(glm::vec2 pos) {

	//for (const PBody& body : bodies) {
	for (int i=bodies.size()-1; i>=0; i--){
		// Get all axes
		std::vector<glm::vec2> axes = bodies[i]->getAxes();
		if (axes.size() == 0)
			break;

		bool inObject = true;

		// SAT for a single point
		for (glm::vec2 axis : axes) {
			// Project the shape onto the axis
			glm::vec2 proj = bodies[i]->project(axis);

			// Project the mouse position onto the axis
			float posProj = glm::dot(pos, axis);

			// Check if the mouse pos projection is outside of the shape projection
			if (posProj < proj.x || posProj > proj.y) {
				// If it is, then the mouse pos is not within the shape
				inObject = false;
				break;
			}
		}

		// If the mouse pos is in the shape
		if (inObject) {
			// Return that shape
			return bodies[i];
		}
	}

	return PBody(nullptr);

}

void Simulation::pausePlay() {
	playing = !playing;
}

void Simulation::pause() {
	playing = false;
}

void Simulation::play() {
	playing = true;
}

void Simulation::clearSimulation(){
	bodies = std::vector<PBody>();
}

// Adding Objects:
void Simulation::addBody(const Body& body) {
	bodies.push_back(std::make_shared<Body>(body));
}

void Simulation::addRigidbody(const Rigidbody& body) {
	bodies.push_back(std::make_shared<Rigidbody>(body));
}

void Simulation::addRectangle(const Rectangle& body) {
	bodies.push_back(std::make_shared<Rectangle>(body));
}

void Simulation::addSquare(const Square& body) {
	bodies.push_back(std::make_shared<Square>(body));
}

void Simulation::addCircle(const Circle& body) {
	bodies.push_back(std::make_shared<Circle>(body));
}

void Simulation::addSoftbody(const Softbody& body) {
	bodies.push_back(std::make_shared<Softbody>(body));
}


// Pointer Getters
bool* Simulation::getCollisionsP() {
	return &collisions;
}

float* Simulation::getElasticityP() {
	return &elasticity;
}

float* Simulation::getSpeed() {
	return &speed;
}