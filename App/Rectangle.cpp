#include "Rectangle.h"
#include "Utilities.h"

Rectangle::Rectangle() : Rigidbody() {

}

Rectangle::Rectangle(double width, double height) : 
	Rigidbody(sizeToVerticies(width, height)), mWidth(width), mHeight(height){

}


Rectangle::Rectangle(double width, double height, glm::vec2 position, glm::vec2 vel, glm::vec2 Force, double Mass, ImVec4 colour, bool stat) :
	Rigidbody(sizeToVerticies(width, height), position, vel, Force, Mass, colour, stat), mWidth(width), mHeight(height) {

}

std::vector<glm::vec2> Rectangle::sizeToVerticies(double width, double height) {
	std::vector<glm::vec2> vert = std::vector<glm::vec2>();
	double h =  height / 2;
	double w =  width / 2;
	vert.push_back(glm::vec2(-w, -h));
	vert.push_back(glm::vec2(w, -h));
	vert.push_back(glm::vec2(w, h));
	vert.push_back(glm::vec2(-w, h));
	return vert;
}

std::string Rectangle::save() {

	std::ostringstream os;
	os << "rectangle ";
	os << utilities::vec_to_str(position) << " ";
	os << utilities::vec_to_str(velocity) << " ";
	os << utilities::vec_to_str(force) << " ";
	os << utilities::to_str(mass) << " ";
	os << utilities::imvec_to_str(colour) << " ";
	os << utilities::to_str(isStatic) << " ";
	os << utilities::to_str(mWidth) << " ";
	os << utilities::to_str(mHeight);

	return os.str();

}

Rectangle Rectangle::loadRectangle(std::vector<std::string> data){
	glm::vec2 pos = utilities::str_to_vec(data[1]);
	glm::vec2 vel = utilities::str_to_vec(data[2]);
	glm::vec2 frc = utilities::str_to_vec(data[3]);
	float mss = utilities::str_to_float(data[4]);
	ImVec4 col = utilities::str_to_imvec(data[5]);
	bool stat = utilities::str_to_bool(data[6]);
	float wid = utilities::str_to_float(data[7]);
	float hei = utilities::str_to_float(data[8]);
	return Rectangle(wid, hei, pos, vel, frc, mss, col, stat);
}