#pragma once

#include "glm/glm.hpp"
#include <vector>
#include "imguiInclude.h"

#include "Body.h"

class Rigidbody : public Body {

protected:
	std::vector<glm::vec2> verticies;
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 force;
	double mass;
	ImVec4 colour;
	bool isStatic;

public:

	Rigidbody();
	Rigidbody(std::vector<glm::vec2> vert);
	Rigidbody(std::vector<glm::vec2> vert, glm::vec2 pos, glm::vec2 vel, glm::vec2 Force, double Mass, ImVec4 col, bool stat);

	void update(double dt) override;
	void render(Renderer* renderer) override;
	Projection project(glm::vec2 axis) override;
	
};