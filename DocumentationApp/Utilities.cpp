#include "Utilities.h"

std::string utilities::vec_to_str(const glm::vec2& vec) {
	std::ostringstream os;
	os << "{" << vec.x << "," << vec.y << "}";
	return os.str();
}

std::string utilities::imvec_to_str(const ImVec4& vec) {
	std::ostringstream os;
	os << "-" << vec.x << "," << vec.y << "," << vec.z << "," << vec.w << "-";
	return os.str();
}

float utilities::str_to_float(std::string str) {
	return std::stof(str);
}

glm::vec2 utilities::str_to_vec(std::string str) {
	// Validation
	if (str[0] != '{' || str[str.size() - 1] != '}') {
		std::cout << "str_to_vec: ERROR IN SAVE FILE" << std::endl;
		return glm::vec2();
	}

	glm::vec2 vec = glm::vec2();
	std::string current = std::string();
	for (int i = 1; i < str.size(); i++) {
		if (str[i] == ',') {
			vec.x = str_to_float(current);
			current = "";
		}
		else if (str[i] == '}') {
			vec.y = str_to_float(current);
			current = "";
		}
		else {
			current.push_back(str[i]);
		}
	}
	return vec;
}

ImVec4 utilities::str_to_imvec(std::string str) {
	// Validation
	if (str[0] != '-' || str[str.size() - 1] != '-') {
		std::cout << "str_to_imvec: ERROR IN SAVE FILE" << std::endl;
		return ImVec4();
	}

	ImVec4 vec = ImVec4();
	std::string current = std::string();
	int num = 0;
	for (int i = 1; i < str.size(); i++) {
		if (str[i] == '-') {
			vec.w = str_to_float(current);
		}
		else if (str[i] == ',') {
			if (num == 0) {
				vec.x = str_to_float(current);
			}
			else if (num == 1) {
				vec.y = str_to_float(current);
			}
			else if (num == 2) {
				vec.z = str_to_float(current);
			}
			num++;
			current = "";
		}
		else {
			current.push_back(str[i]);
		}
	}

	return vec;
}

bool utilities::str_to_bool(std::string str) {
	return std::stoi(str);
}

glm::vec2 utilities::imvec2_to_vec(ImVec2 vec) {
	return glm::vec2(vec.x, vec.y);
}

ImVec2 utilities::vec_to_imvec2(glm::vec2 vec) {
	return ImVec2(vec.x, vec.y);
}

