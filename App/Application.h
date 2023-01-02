#pragma once

#include "Simulation.h"
#include "Renderer.h"

class Application {
private:

	GLFWwindow* window;

public:

	Application();
	~Application();

	void run();
	bool isSetup();

private:

	void setupWindow();

	enum errorType {
		NONE, GLFW_INIT, GLFW_WINDOW, GLEW_INIT
	} error;

};

