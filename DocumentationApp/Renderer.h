#pragma once

#define NOMINMAX

#include "imguiInclude.h"
#include "Simulation.h"

#include "Circle.h"
#include "Square.h"

#include "openglAbstraction/Framebuffer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui-filebrowser/imfilebrowser.h"

class Renderer {

public:

	Renderer(GLFWwindow* win, Simulation* sim);
	void newFrame();
	void renderImGUI();
	void renderSimulation();

	void drawCircle(glm::vec2 position, double radius, ImVec4 colour);
	void drawPolygon(glm::vec2 position, std::vector<glm::vec2> vertices, ImVec4 colour);
	void drawLine(glm::vec2 pos1, glm::vec2 pos2, ImVec4 colour);
	void drawArrow(glm::vec2 pos1, glm::vec2 pos2, ImVec4 colour);

	ImVec2 getWindowSize();

private:

	// Window Pointer
	GLFWwindow* window;

	// Simulation class Pointer
	Simulation* simulation;


	// Window properties
	ImVec2 wsize;
	ImVec2 pos;

	ImVec4 background = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); // Background colour

	// Simulation Window
	PBody focusedBody = PBody(nullptr);

	FrameBuffer fb = FrameBuffer(); // Framebuffer
	glm::mat4 projection; // Projection Matrix
	bool velocityArrows = false;

	void handleSimulationInputs();

	// ImGUI windows
	bool showGeneralInformation = true;
	bool showAddObject = true;
	bool showObjectInformation = true;
	bool showHelpMenu = false;
	bool showSaveWindow = false;
	bool showLoadWindow = false;

	// Render Panels
	void renderGeneralInformationPanel();
	void renderAddObject();
	void renderObjectInformation();
	void renderHelpMenu();
	void renderSaveWindow();
	void renderLoadWindow();
	void renderMenuBar();

	// Add object panel
	const char* bodyTypes[6] = { "Body", "Rigidbody", "Rectangle", "Square", "Circle", "Softbody" };
	int selectedBodyType = 1;

	void renderAddBody();
	void renderAddRigidbody();
	void renderAddRectangle();
	void renderAddSquare();
	void renderAddCircle();
	void renderAddSoftbody();

	ImGui::FileBrowser saveWindow;
	ImGui::FileBrowser loadWindow;

};