#pragma once

#include "Body.h"

#include <vector>
#include "glm/glm.hpp"

class Rigidbody : public Body {
public:
	
	Rigidbody();
	Rigidbody(std::vector<glm::vec2> vert);
	Rigidbody(std::vector<glm::vec2> vert, glm::vec2 pos, glm::vec2 vel, glm::vec2 Force, double Mass, ImVec4 col, bool stat);

	void update(double dt) override;
	void render(Renderer* renderer) override;

	std::string save() override;
	static Rigidbody loadRigidbody(std::vector<std::string> data);

	std::vector<Body*> getCollisionBodies() override;
	glm::vec2 project(glm::vec2 axis) override;
	std::vector<glm::vec2> getAxes() override;
	std::vector<glm::vec2> getVertices();

protected:

	std::vector<glm::vec2> vertices;

};