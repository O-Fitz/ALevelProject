#include "Circle.h"
#include "Renderer.h"
#include "Utilities.h"

Circle::Circle() :
	radius(1), Rigidbody() {

}

Circle::Circle(double rad, glm::vec2 pos, glm::vec2 vel, glm::vec2 Force, double Mass, ImVec4 col, bool stat) :
	radius(rad), Rigidbody(std::vector<glm::vec2>(), pos, vel, Force, Mass, col, stat) {

}

void Circle::render(Renderer* renderer) {
	renderer->drawCircle(position, radius, colour);
}

glm::vec2 Circle::project(glm::vec2 axis) {
	
	// Project position onto axis
	float dotProduct = glm::dot<float>(position, axis);

	// Add and subtract radius to find min and max
	return glm::vec2(dotProduct - radius, dotProduct + radius);
}

std::vector<glm::vec2> Circle::getAxes() {
	std::vector<glm::vec2>axes = std::vector<glm::vec2>();

	// Iterate through angles in radians
	for (float i = 0; i < 2 * 3.141; i += 2 * 3.141 / collisionAxis) {
		// x = cos(i)
		// y = sin(i)
		// Dont need to multiply by the radius as the axis is only a direction
		axes.push_back(glm::normalize(glm::vec2(cos(i), sin(i))));
	}

	axes.push_back(glm::vec2(0, 1));
	return axes;
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