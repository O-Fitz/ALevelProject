#pragma once
#include "imguiInclude.h"
#include "Simulation.h"
#include "Renderer.h"

class Application {
public:

	Application();
	~Application();

	void run();
	bool isSetup();

private:

	GLFWwindow* window;
	Simulation* simulation;

	enum errorType {
		NONE, GLFW_INIT, GLFW_WINDOW, GLEW_INIT
	} error;


	void setupWindow();

};