#pragma once

#include "glm/glm.hpp"
#include "imguiInclude.h"

class Renderer;

class Tracer {
public:
	Tracer();
	Tracer(glm::vec2 position, double timeLeft, ImVec4 colour);

	void render(Renderer* renderer);
	void update(double dt);
	bool getDestroy() const;
private:
	glm::vec2 position;
	double timeLeft;
	ImVec4 colour;
};