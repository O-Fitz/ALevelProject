#pragma once

#include "definitions.h"

#include "imguiInclude.h"
#include <vector>
#include "Body.h"
#include "Simulation.h"

#include <iostream>
#include <algorithm>

#include "openglAbstraction/Framebuffer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Renderer {
private:

	GLFWwindow* window;

	// Window Properties
	ImVec2 wsize;
	ImVec2 pos;

	// FrameBuffer for rendering to imgui window
	FrameBuffer fb = FrameBuffer();
	glm::mat4 projection;

	// OPTIONS:
	ImVec4 background = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); // Background colour

	bool show_demo_window = true; // Temporary windows
	bool show_another_window = false;
	bool show_simulation_window = true;

	// Add object values
	int selected_type_index;
	const char* bodyTypes[9] = { "Rigidbody", "N-Gon", "Circle", "Semi-Circle", "Triangle", "Rectangle", "Square", "Line", "Softbody" };

	// Simulation
	Simulation* simulation;
public:

	Renderer(GLFWwindow* win, Simulation* sim);
	void newFrame();
	void renderImGUI();
	void renderSimulation();

	void renderCircle(glm::vec2 position, double radius, ImVec4 colour);
	void renderPolygon(glm::vec2 position, std::vector<glm::vec2> verticies, ImVec4 colour);

	ImVec2 getWindowSize();

private:

	void addBodyOptions();

};