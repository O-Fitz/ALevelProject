#include "Rigidbody.h"

Rigidbody::Rigidbody() {
	verticies = std::vector<Vector>();
	position = Vector();
	velocity = Vector();
	force = Vector();
	mass = 1.0;
	colour = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

}

Rigidbody::Rigidbody(std::vector<Vector> vert) : verticies(vert) {
	position = Vector();
	velocity = Vector();
	force = Vector();
	mass = 1.0;
	colour = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
}

Rigidbody::Rigidbody(std::vector<Vector> Vert, Vector Pos, Vector Vel, Vector Force, double Mass, ImVec4 col) :
	verticies(Vert), position(Pos), velocity(Vel), force(Force), mass(Mass), colour(col) {

}

void Rigidbody::render(Renderer* renderer) {

	renderer->renderPolygon(position, verticies, colour);

}