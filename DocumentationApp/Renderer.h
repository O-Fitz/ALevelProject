#pragma once

#include "imguiInclude.h"
#include "Simulation.h"

class Renderer {

public:

	Renderer(GLFWwindow* win, Simulation* sim);
	void newFrame();
	void renderImGUI();
	void renderSimulation();

	void renderCircle(glm::vec2 position, double radius, ImVec4 colour);
	void renderPolygon(glm::vec2 position, std::vector<glm::vec2> verticies, ImVec4 colour);

	ImVec2 getWindowSize();

private:

	// Window Pointer
	GLFWwindow* window;

	// Simulation Pointer;
	Simulation* simulation;
	
	// Window properties
	ImVec2 wsize;
	ImVec2 pos;


	ImVec4 background = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); // Background colour

};