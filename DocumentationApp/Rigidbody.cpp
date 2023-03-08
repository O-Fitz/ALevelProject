#include "Rigidbody.h"
#include "Renderer.h"

Rigidbody::Rigidbody() {
	verticies = std::vector<glm::vec2>();
	position = glm::vec2();
	velocity = glm::vec2();
	force = glm::vec2();
	mass = 1.0;
	colour = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
}

Rigidbody::Rigidbody(std::vector<glm::vec2> vert) : verticies(vert) {
	position = glm::vec2();
	velocity = glm::vec2();
	force = glm::vec2();
	mass = 1.0;
	colour = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
}

Rigidbody::Rigidbody(std::vector<glm::vec2> Vert, glm::vec2 Pos, glm::vec2 Vel, glm::vec2 Force, double Mass, ImVec4 col, bool stat) :
	verticies(Vert), Body(Pos, Vel, Force, Mass, col, stat) {

}

void Rigidbody::render(Renderer* renderer) {
	renderer->renderPolygon(position, verticies, colour);
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