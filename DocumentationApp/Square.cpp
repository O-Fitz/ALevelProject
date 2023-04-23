#include "Square.h"
#include "Utilities.h"

Square::Square() : Rectangle(1, 1), sideLength(1) {

}

Square::Square(double sideLength) : Rectangle(sideLength, sideLength), sideLength(sideLength) {

}

Square::Square(double sideLength, glm::vec2 position, glm::vec2 vel, glm::vec2 Force, double Mass, ImVec4 colour, bool stat) :
	Rectangle(sideLength, sideLength, position, vel, Force, Mass, colour, stat), sideLength(sideLength) {


}

std::string Square::save() {
	std::ostringstream os;
	os << "square ";
	os << utilities::vec_to_str(position) << " ";
	os << utilities::vec_to_str(velocity) << " ";
	os << utilities::vec_to_str(force) << " ";
	os << utilities::to_str(mass) << " ";
	os << utilities::imvec_to_str(colour) << " ";
	os << utilities::to_str(isStatic) << " ";
	os << utilities::to_str(sideLength);

	return os.str();
}

Square Square::loadSquare(std::vector<std::string> data) {
	glm::vec2 pos = utilities::str_to_vec(data[1]);
	glm::vec2 vel = utilities::str_to_vec(data[2]);
	glm::vec2 frc = utilities::str_to_vec(data[3]);
	float mss = utilities::str_to_float(data[4]);
	ImVec4 col = utilities::str_to_imvec(data[5]);
	bool stat = utilities::str_to_bool(data[6]);
	float len = utilities::str_to_float(data[7]);
	return Square(len, pos, vel, frc, mss, col, stat);
}

double Square::getSideLength() {
	return sideLength;
}