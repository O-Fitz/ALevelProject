#include "Rigidbody.h"
#include "Renderer.h"
#include "Utilities.h"
#include <algorithm>

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
	renderer->drawPolygon(position, vertices, colour);
}

void Rigidbody::update(double dt) {

	// Check if object is static
	if (isStatic) {
		// If so exit out of the function
		return;
	}

	velocity += (force / mass * (float)dt + impulse / mass); // Update the velocity
	impulse = glm::vec2(0, 0); // Set impulse to (0, 0)
	position += velocity * (float)dt; // Update the position

}

std::string Rigidbody::save() {
	std::ostringstream os;
	os << "rigidbody ";
	os << utilities::vec_to_str(position) << " ";
	os << utilities::vec_to_str(velocity) << " ";
	os << utilities::vec_to_str(force) << " ";
	os << utilities::to_str(mass) << " ";
	os << utilities::imvec_to_str(colour) << " ";
	os << utilities::to_str(isStatic) << " ";
	os << "(";
	for (int i = 0; i < vertices.size(); i++) {
		os << utilities::vec_to_str(vertices[i]);
		if (i != vertices.size() - 1) {
			os << ",";
		}
	}
	os << ")";

	return os.str();
}

Rigidbody Rigidbody::loadRigidbody(std::vector<std::string> data) {
	glm::vec2 pos = utilities::str_to_vec(data[1]);
	glm::vec2 vel = utilities::str_to_vec(data[2]);
	glm::vec2 frc = utilities::str_to_vec(data[3]);
	float mss = utilities::str_to_float(data[4]);
	ImVec4 col = utilities::str_to_imvec(data[5]);
	bool stat = utilities::str_to_bool(data[6]);

	// Split the list of vec2s into individual vec2s
	// Uses similar system to splitting up line
	std::vector<glm::vec2> vert = std::vector<glm::vec2>();
	std::string current = std::string();
	for (int i = 0; i < data[7].size(); i++) {
		if (data[7][i] == ',' || data[7][i] == ')') {
			vert.push_back(utilities::str_to_vec(current));
			current = "";
		}
		else {
			current.push_back(data[7][i]);
		}
	}

	return Rigidbody(vert, pos, vel, frc, mss, col, stat);
}

std::vector<Body*> Rigidbody::getCollisionBodies() {
	return std::vector<Body*>({this});
}

glm::vec2 Rigidbody::project(glm::vec2 axis) {
	// Set minA to maximum value (so that any other values will be less than it)
	float minA = std::numeric_limits<float>::infinity();
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
		axes.push_back(glm::normalize(glm::vec2(edge.y, -edge.x)));
	}

	return axes;

}

std::vector<glm::vec2> Rigidbody::getVertices() {

	std::vector<glm::vec2> ver = std::vector<glm::vec2>();
	for (const glm::vec2& vertex : vertices) {
		ver.push_back(vertex + position);
	}

	return ver;
}