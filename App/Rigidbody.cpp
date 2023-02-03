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

	if (isStatic) {
		return;
	}

	/*glm::vec2 dpos = glm::vec2(500, 500) - position;
	force = glm::normalize(dpos)*glm::fvec1(mass*100);*/

	glm::fvec1 dtv = glm::fvec1(dt);
	velocity += (force / glm::fvec1(mass)) * dtv + impulse / glm::fvec1(mass);
	impulse = glm::vec2(0, 0);
	position += velocity * dtv;
	//force = glm::vec2(0, 0);

}

std::vector<glm::vec2> Rigidbody::getAxes() {
	std::vector<glm::vec2> axes = std::vector<glm::vec2>();

	for (int i = 0; i < verticies.size(); i++) {
		glm::vec2 face = verticies[(i + 1) % verticies.size()] - verticies[i];
		glm::vec2 perp = glm::vec2(face.y, -face.x);

		axes.push_back(perp);
	}

	return axes;

}

Projection Rigidbody::project(glm::vec2 axis) {

	float minA = std::numeric_limits<float>::max();
	float maxA = -minA;
	for (const auto& vertex : verticies) {
		float dotProduct = glm::dot<float>(vertex + position, axis);
		minA = std::min(minA, dotProduct);
		maxA = std::max(maxA, dotProduct);
	}

	return Projection(axis, minA, maxA);
}