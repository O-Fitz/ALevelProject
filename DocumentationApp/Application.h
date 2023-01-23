#pragma once
#include "imguiInclude.h"

class Application {
public:

	Application();
	~Application();

	void run();
	bool isSetup();

private:

	GLFWwindow* window;

	enum errorType {
		NONE, GLFW_INIT, GLFW_WINDOW, GLEW_INIT
	} error;


	void setupWindow();

};