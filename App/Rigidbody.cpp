#include "Rigidbody.h"
#include "Renderer.h"

#include "Utilities.h"
#include <complex>

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

	if (isStatic) {
		return;
	}

	/*glm::vec2 dpos = glm::vec2(500, 500) - position;
	force = glm::normalize(dpos)*glm::fvec1(mass*100);*/

	glm::fvec1 dtv = glm::fvec1(dt);
	velocity += (force / glm::fvec1(mass)) * dtv + impulse / glm::fvec1(mass);
	impulse = glm::vec2(0, 0);
	position += velocity * dtv;

	angularVelocity += angularImpulse / mass;
	angularImpulse = 0;
	angle += angularVelocity * dt;


	//force = glm::vec2(0, 0);

}

std::vector<glm::vec2> Rigidbody::getAxes() {
	std::vector<glm::vec2> axes = std::vector<glm::vec2>();

	for (int i = 0; i < vertices.size(); i++) {
		glm::vec2 face = vertices[(i + 1) % vertices.size()] - vertices[i];
		std::complex<float> comp = std::complex<float>(face.x, face.y);
		comp *= std::complex<float>(cos(3.141 + angle), sin(3.141 + angle));
		glm::vec2 perp = glm::vec2(comp.real(), comp.imag());

		axes.push_back(glm::normalize(perp));
	}

	return axes;

}

Projection Rigidbody::project(glm::vec2 axis) {

	float minA = std::numeric_limits<float>::max();
	float maxA = -minA;
	for (const auto& vertex : vertices) {
		float dotProduct = glm::dot<float>(vertex + position, axis);
		minA = std::min(minA, dotProduct);
		maxA = std::max(maxA, dotProduct);
	}

	return Projection(axis, minA, maxA);
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