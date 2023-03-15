#include "Simulation.h"
#include "Renderer.h"

Simulation::Simulation() {
	bodies = std::vector<PBody>();

	glm::vec2 pos = glm::vec2(100, 100);
	glm::vec2 pos2 = glm::vec2(200, 200);
	glm::vec2 vel = glm::vec2(100, 100);
	glm::vec2 vel2 = glm::vec2(0, 0);

	Circle c1 = Circle(20, pos, vel, glm::vec2(0, 0), 2, ImVec4(0, 255, 0, 255), false);
	Square s1 = Square(20, pos2, vel2, glm::vec2(0, 0), 2, ImVec4(255, 0, 0, 255), false);

	Square s2 = Square(20, pos2, vel2, vel2, 2, ImVec4(0, 255, 0, 255), false);
	Square s3 = Square(20, pos2+glm::vec2(6, 5), vel2, vel2, 2, ImVec4(255, 0, 0, 255), false);
	
	bodies.push_back(std::make_shared<Square>(s2));
	bodies.push_back(std::make_shared<Square>(s3));

	//bodies.push_back(std::make_shared<Circle>(c1));
	//bodies.push_back(std::make_shared<Square>(s1));

}

Simulation::~Simulation() {

}

void Simulation::update(double dt) {

	// Call update functions
	for (int i = 0; i < bodies.size(); i++) {
		bodies[i]->update(dt);
	}

	// Collision Detection
	collisionDetection();
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
			if (!checked[j] && checkCollision(collisionBodies[i], collisionBodies[j])) {
				collisionResloution(collisionBodies[i], collisionBodies[j]);
			}
		}
	}
}

bool Simulation::checkCollision(Body* b1, Body* b2) {
	// Find all of the axes and append them to one vector
	std::vector<glm::vec2> axes = b1->getAxes();
	std::vector<glm::vec2> b2axes = b2->getAxes();
	axes.insert(std::end(axes), std::begin(b2axes), std::end(b2axes));


	// Iterate through axes
	for (const glm::vec2& axis : axes) {
		// Project shapes onto axis
		glm::vec2 p1 = b1->project(axis);
		glm::vec2 p2 = b2->project(axis);

		// If they don't overlap, Bodies dont collide
		if (p1.x > p2.y || p1.y < p2.x) {
			return false;
		}
	}

	return true;
}

void Simulation::collisionResloution(Body* b1, Body* b2) {
	positionCorrection((Rigidbody*)b1, (Rigidbody*)b2);
}

void Simulation::positionCorrection(Rigidbody* b1, Rigidbody* b2) {

	

	if (b1->getStatic() && b2->getStatic()) {
		return;
	}

	glm::vec2 mostPenetrating = glm::vec2(0, 0);
	float distance = std::numeric_limits<float>::max();
	Rigidbody* body = 0;

	// Finds the most penetrating vertex
	if (findMostPenetratingVertex(b1, b2, mostPenetrating, distance)) 
		body = b2;
	
	if (findMostPenetratingVertex(b2, b1, mostPenetrating, distance))
		body = b1;

	// Find closest face

	glm::vec2 moveVector = findDisplacementVector(body, mostPenetrating);

	std::cout << "Distance: " << distance << std::endl;
	std::cout << "Most Penetrating: " << mostPenetrating.x  << " " << mostPenetrating.y << std::endl;
	std::cout << "MoveVector: " << moveVector.x << " " << moveVector.y << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	// Move objects

	if (body == b2) {
		moveVector = -moveVector;
	}

	if (b1->getStatic()) {
		b2->addPosition(moveVector);
	}
	else if (b2->getStatic()) {
		b1->addPosition(-moveVector);
	}
	else {
		b1->addPosition(moveVector * glm::fvec1(0.5));
		b2->addPosition(-moveVector * glm::fvec1(0.5));
	}

}

bool Simulation::findMostPenetratingVertex(Rigidbody* b1, Rigidbody* b2, glm::vec2& mostPenetrating, float& distance) {
	// Passes mostPenetrating and distance by reference

	bool hasMostPenVertex = false;

	for (const glm::vec2& vertex : b1->getVertices()) {
		// Is penetrating
		bool pen = true;
		
		// Check if the vertex is penetrating (uses similar process to SAT)
		for (const glm::vec2& axis : b2->getAxes()) {
			// Projects b2 onto the axes
			glm::vec2 proj = b2->project(axis);
			// Projects the vertex onto the axes
			float dot = glm::dot<float>(vertex, axis);

			// Checks if the vertex is outside the shadow
			if (proj.x > dot || proj.y < dot) {
				// If so, then the vertex is not penetrating
				pen = false;
				break;
			}
		}

		if (pen) {
			// If the object is penetrating
			// Find the distance from the vertex to the centre of the other shape
			glm::vec2 relPos = b2->getPostition() - vertex;
			float dis = glm::distance<float>(relPos.x, relPos.y);
			if (dis < distance) {
				// If the distance is closer than the previous closer one
				// Set the distance and mostPenetrating to be the vertex and the distance
				distance = dis;
				mostPenetrating = vertex;
				hasMostPenVertex = true;
			}
		}
	}

	return hasMostPenVertex;

}

glm::vec2 Simulation::findDisplacementVector(Rigidbody* b1, glm::vec2 vertex) {

	std::vector<glm::vec2> vertices = b1->getVertices();

	glm::vec2 shortest = glm::vec2(10000000000, 10000000000);
	std::cout << "Shortest1: " << shortest.x << " " << shortest.y << std::endl;

	for (int i = 0; i < vertices.size(); i++) {
		glm::vec2 AB = vertices[(i + 1) % vertices.size()] - vertices[i];
		glm::vec2 AP = vertex - vertices[i];

		float u = glm::dot<float>(AB, AP)/(glm::length(AB)*glm::length(AB));
		
		glm::vec2 test = (vertices[i] + glm::fvec1(u) * AB);
		glm::vec2 vec = vertex - (vertices[i] + glm::fvec1(u) * AB);

		std::cout << "AB: " << AB.x << " " << AB.y << std::endl;
		std::cout << "AP: " << AP.x << " " << AP.y << std::endl;
		std::cout << "U: " << u << std::endl;
		std::cout << "Vertex: " << vertex.x << " " << vertex.y << std::endl;
		std::cout << "A + u*AB: " << test.x << " " << test.y << std::endl;
		std::cout << "Vec: " << vec.x << " " << vec.y << std::endl;
		std::cout << std::endl;


		if (glm::length(vec) < glm::length(shortest)) {
			shortest = vec;
		}
	}

	std::cout << "Shortest: " << shortest.x << " " << shortest.y << std::endl;

	return shortest;

}