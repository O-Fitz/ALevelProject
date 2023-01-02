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
	verticies(Vert), position(Pos), velocity(Vel), force(Force), mass(Mass), colour(col), isStatic(stat) {

}

void Rigidbody::render(Renderer* renderer) {

	renderer->renderPolygon(position, verticies, colour);

}

void Rigidbody::update(double dt) {
	glm::fvec1 dtv = glm::fvec1(dt);
	velocity += (force / glm::fvec1(mass)) * dtv;
	position += velocity * dtv;

}

Projection Rigidbody::project(glm::vec2 axis) {

	for (const glm::vec2& vertex : verticies) {
		glm::vec2 vpos = vertex + position;
		glm::vec2 perp = axis - vpos * ((glm::dot(vpos, axis)) / (glm::dot(vpos, vpos)));
	}

	return Projection(axis);
}