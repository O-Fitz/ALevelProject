#include "Softbody.h"
#include "Renderer.h"
#include "Utilities.h"

Softbody::Softbody(glm::vec2 position, glm::vec2 velocity, glm::vec2 force, float mass, ImVec4 colour, bool isStatic, int noPoints, float radius, float springConstant, float dampeningFactor) :
	Body(position, velocity, force, mass, colour, isStatic), noPoints(noPoints), radius(radius), springConstant(springConstant), dampeningFactor(dampeningFactor) {

	points = std::vector<Point>();
	springs = std::vector<Spring>();

	setupPoints();
	setupSprings();

}

void Softbody::setupPoints() {

	for (float i = 0; i < 2 * 3.141; i += 2 * 3.141 / noPoints) {

		glm::vec2 pos = glm::vec2(cos(i), sin(i)) * radius;
		Circle c = Circle(5, pos + position, velocity, force, mass / noPoints, colour, isStatic);
		points.push_back(std::make_shared<Circle>(c));

	}

}

void Softbody::setupSprings() {

	std::vector<bool>added = std::vector<bool>();
	for (int i = 0; i < points.size(); i++) {
		added.push_back(false);
	}

	for (int i = 0; i < points.size(); i++) {
		added[i] = true;
		for (int j = 0; j < points.size(); j++) {
			if (!added[j]) {
				float distance = glm::length(points[i]->getPostition() - points[j]->getPostition());
				springs.emplace_back(points[i], points[j], distance, springConstant, dampeningFactor);

			}
		}
	}

}

void Softbody::update(double dt) {
	if (isStatic) {
		return;
	}

	velocity += (force / mass) * (float)dt + impulse / mass;
	impulse = glm::vec2(0, 0);
	position += velocity * (float)dt;

	for (int i = 0; i < springs.size(); i++) {
		springs[i].update(dt);
	}

	for (int i = 0; i < points.size(); i++) {
		points[i]->update(dt);
	}

}

void Softbody::render(Renderer* renderer) {

	for (int i = 0; i < springs.size(); i++) {
		springs[i].render(renderer, colour);
	}

	for (int i = 0; i < points.size(); i++) {
		points[i]->render(renderer);
	}

}

std::vector<Body*> Softbody::getCollisionBodies() {
	std::vector<Body*> bodies = std::vector<Body*>();

	for (int i = 0; i < points.size(); i++) {
		bodies.push_back(points[i].get());
	}

	return bodies;
}

std::string Softbody::save() {

	std::ostringstream os;
	os << "softbody ";
	os << utilities::vec_to_str(position) << " ";
	os << utilities::vec_to_str(velocity) << " ";
	os << utilities::vec_to_str(force) << " ";
	os << utilities::to_str(mass) << " ";
	os << utilities::imvec_to_str(colour) << " ";
	os << utilities::to_str(isStatic) << " ";
	
	os << utilities::to_str(noPoints) << " ";
	os << utilities::to_str(radius) << " ";
	os << utilities::to_str(springConstant) << " ";
	os << utilities::to_str(dampeningFactor);

	return os.str();

}

Softbody Softbody::loadSoftbody(std::vector<std::string> data) {
	glm::vec2 pos = utilities::str_to_vec(data[1]);
	glm::vec2 vel = utilities::str_to_vec(data[2]);
	glm::vec2 frc = utilities::str_to_vec(data[3]);
	float mss = utilities::str_to_float(data[4]);
	ImVec4 col = utilities::str_to_imvec(data[5]);
	bool stat = utilities::str_to_bool(data[6]);
	
	int np = (int)utilities::str_to_float(data[7]);
	float rad = utilities::str_to_float(data[8]);
	float sc = utilities::str_to_float(data[9]);
	float df = utilities::str_to_float(data[10]);

	return Softbody(pos, vel, frc, mss, col, stat, np, rad, sc, df);
}