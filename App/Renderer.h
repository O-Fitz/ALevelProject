#pragma once

#include "definitions.h"

#include "imguiInclude.h"
#include <vector>
#include "Maths/Vector.h"
#include "Body.h"

class Renderer {
private:

	GLFWwindow* window;

public:

	Renderer(GLFWwindow* win);
	void newFrame(ImVec4& clear_color);
	void renderImGUI(bool& show_demo_window, bool& show_another_window, ImVec4& clear_color);
	void renderSimulation(std::vector<Body> bodies);

	void renderCircle(Vector position, double radius);
	void renderPolygon(Vector position, std::vector<Vector> verticies);
};