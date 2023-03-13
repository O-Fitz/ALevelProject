#include "Rigidbody.h"
#include "Renderer.h"

Rigidbody::Rigidbody() {
	vertices = std::vector<glm::vec2>();
	position = glm::vec2();
	velocity = glm::vec2();
	force = glm::vec2();
	mass = 1.0;
	colour = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
}

Rigidbody::Rigidbody(std::vector<glm::vec2> vert) : vertices(vert) {
	position = glm::vec2();
	velocity = glm::vec2();
	force = glm::vec2();
	mass = 1.0;
	colour = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
}

Rigidbody::Rigidbody(std::vector<glm::vec2> Vert, glm::vec2 Pos, glm::vec2 Vel, glm::vec2 Force, double Mass, ImVec4 col, bool stat) :
	vertices(Vert), Body(Pos, Vel, Force, Mass, col, stat) {

}

void Rigidbody::render(Renderer* renderer) {
	renderer->renderPolygon(position, vertices, colour);
}

void Rigidbody::update(double dt) {

	// Check if object is static
	if (isStatic) {
		// If so exit out of the function
		return;
	}

	glm::fvec1 dtv = glm::fvec1(dt); // A vector version of dt
	velocity += (force / glm::fvec1(mass)) * dtv + impulse / glm::fvec1(mass); // Update the velocity
	impulse = glm::vec2(0, 0); // Set impulse to (0, 0)
	position += velocity * dtv; // Update the position

}

std::vector<Body*> Rigidbody::getCollisionBodies() {
	return std::vector<Body*>({this});
}

glm::vec2 Rigidbody::project(glm::vec2 axis) {
	// Set minA to maximum value (so that any other values will be less than it)
	float minA = std::numeric_limits<float>::max();
	// Set maxA to minimum value (so that any other values will be greater than it)
	float maxA = -minA;

	for (const auto& vertex : vertices) {
		// Dot product calculates how far along the axis the vertex is projected
		float dotProduct = glm::dot<float>(vertex + position, axis);

		// Compares dotProduct to min and max values
		minA = std::min(minA, dotProduct);
		maxA = std::max(maxA, dotProduct);
	}

	return glm::vec2(minA, maxA); 
}

std::vector<glm::vec2> Rigidbody::getAxes() {

	// Container for the axes
	std::vector<glm::vec2> axes = std::vector<glm::vec2>();

	// Iterate through axes
	for (int i = 0; i < vertices.size(); i++) {
		// Subtract the vertex from the next
		// The "% vertices.size()" is so that the edge goes between the first vertex and the last
		glm::vec2 edge = vertices[(i + 1) % vertices.size()] - vertices[i];

		// Find the perpendicular vector by swapping x and y and making x negative
		// and add it to the axes.
		axes.push_back(glm::vec2(edge.y, -edge.x));
	}

	return axes;

}