#include "Circle.h"
#include "Renderer.h"
#include "Utilities.h"

Circle::Circle() : 
	radius(1), Rigidbody() {

}

Circle::Circle(float rad, glm::vec2 pos, glm::vec2 vel, glm::vec2 Force, float Mass, ImVec4 col, bool stat) :
	radius(rad), Rigidbody(std::vector<glm::vec2>(), pos, vel, Force, Mass, col, stat) {

}


void Circle::render(Renderer* renderer) {
	renderer->drawCircle(position, radius, colour);
}

Projection Circle::project(glm::vec2 axis) {
	glm::vec2 pos1 = position - glm::normalize(axis) * glm::fvec1(radius);
	glm::vec2 pos2 = position + glm::normalize(axis) * glm::fvec1(radius);
	float val1 = glm::dot<float>(pos1, axis);
	float val2 = glm::dot<float>(pos2, axis);
	
	return Projection(axis, val1, val2);
}

void Circle::print() {
	std::cout << "Circle" << std::endl;
}

std::vector<glm::vec2> Circle::getAxes() {
	std::vector<glm::vec2>axes = std::vector<glm::vec2>();
	for (float i = 0; i < 2*PI; i+=2*PI/collisionAxis) {
		axes.push_back(glm::normalize(glm::vec2( sin(i), cos(i))));
	}
	return axes;
}

float Circle::getRadius() {
	return radius;
}

std::string Circle::save() {
	std::ostringstream os;
	os << "circle ";
	os << utilities::vec_to_str(position) << " ";
	os << utilities::vec_to_str(velocity) << " ";
	os << utilities::vec_to_str(force) << " ";
	os << utilities::to_str(mass) << " ";
	os << utilities::imvec_to_str(colour) << " ";
	os << utilities::to_str(isStatic) << " ";
	os << utilities::to_str(radius);

	return os.str();
}

Circle Circle::loadCircle(std::vector<std::string> data) {
	glm::vec2 pos = utilities::str_to_vec(data[1]);
	glm::vec2 vel = utilities::str_to_vec(data[2]);
	glm::vec2 frc = utilities::str_to_vec(data[3]);
	float mss = utilities::str_to_float(data[4]);
	ImVec4 col = utilities::str_to_imvec(data[5]);
	bool stat = utilities::str_to_bool(data[6]);
	float rad = utilities::str_to_float(data[7]);
	return Circle(rad, pos, vel, frc, mss, col, stat);
}