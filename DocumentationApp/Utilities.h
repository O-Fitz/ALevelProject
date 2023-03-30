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

	std::string vec_to_str(const glm::vec2& vec);

	std::string imvec_to_str(const ImVec4& vec);

	float str_to_float(std::string str);

	glm::vec2 str_to_vec(std::string str);

	ImVec4 str_to_imvec(std::string str);

	bool str_to_bool(std::string str);

	glm::vec2 imvec2_to_vec(ImVec2 vec);

	ImVec2 vec_to_imvec2(glm::vec2 vec);
}