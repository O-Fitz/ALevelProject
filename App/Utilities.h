#pragma once

#include <iostream>
#include <sstream>
#include <string>

#include "glm/glm.hpp"
#include "imguiInclude.h"

namespace utilities {



	template < typename Type >
	std::string to_str(const Type& t)
	{
		std::ostringstream os;
		os << t;
		return os.str();
	}

	std::string vec_to_str(const glm::vec2& vec) {
		std::ostringstream os;
		os << "{" << vec.x << "," << vec.y << "}";
		return os.str();
	}

	std::string imvec_to_str(const ImVec4& vec) {
		std::ostringstream os;
		os << "-" << vec.x << "," << vec.y << "," << vec.z << "," << vec.w << "-";
		return os.str();
	}

}