#include "Rigidbody.h"

Rigidbody::Rigidbody() {
	std::vector<Vector> verticies = std::vector<Vector>();
	Vector position = Vector();
	Vector velocity = Vector();
	Vector force = Vector();
	double mass = 1.0;
}

Rigidbody::Rigidbody(std::vector<Vector> Vert, Vector Pos, Vector Vel, Vector Force, double Mass) :
	verticies(Vert), position(Pos), velocity(Vel), force(Force), mass(Mass) {



}

void Rigidbody::render(Renderer* renderer) {

}