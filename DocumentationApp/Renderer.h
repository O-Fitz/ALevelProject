#pragma once

#include "imguiInclude.h"
#include "Simulation.h"

#include "Circle.h"
#include "Square.h"

#include "openglAbstraction/Framebuffer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Renderer {

public:

	Renderer(GLFWwindow* win, Simulation* sim);
	void newFrame();
	void renderImGUI();
	void renderSimulation();

	void drawCircle(glm::vec2 position, double radius, ImVec4 colour);
	void drawPolygon(glm::vec2 position, std::vector<glm::vec2> vertices, ImVec4 colour);
	void drawLine(glm::vec2 pos1, glm::vec2 pos2, ImVec4 colour);

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

	// Framebuffer
	FrameBuffer fb = FrameBuffer();
	glm::mat4 projection;

	// ImGUI windows
	bool showDataWindow = true;
	bool showGeneralInformation = true;

	void renderGeneralInformationPanel();

};