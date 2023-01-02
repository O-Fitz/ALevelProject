#include "Application.h"
#include <string>
#include <iostream>
#include "Renderer.h"
#include "Simulation.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}



Application::Application() {
	setupWindow();
}

Application::~Application() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

//void Application::run() {
//
//	while (!glfwWindowShouldClose(window)) {
//
//
//
//	}
//
//}

void Application::run() {
	Simulation simulation = Simulation();
	Renderer renderer = Renderer(window, &simulation);

	double dt;

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();

		// Set simulation size
		ImVec2 rsize = renderer.getWindowSize();
		glm::vec2 size = glm::vec2(rsize.x, rsize.y);
		simulation.setSimulationSize(size);

		// Update simulation
		dt = ImGui::GetIO().DeltaTime;
		simulation.update(dt);


		// Render the simulation
		renderer.newFrame();
		renderer.renderSimulation();
		renderer.renderImGUI();

		glfwSwapBuffers(window);

	}

}

bool Application::isSetup() {
	std::string errorMessage = "";
	switch (error) {
	case (NONE):
		return true;
	case (GLFW_INIT):
		errorMessage = "Problem with GLFW init";
	case (GLFW_WINDOW):
		errorMessage = "Problem with GLFW window";
	case (GLEW_INIT):
		errorMessage = "Problem with GLEW init";
	}

	std::cout << "Error: " << errorMessage << std::endl;

	return false;
}


void Application::setupWindow() {
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()) {
		error = errorType::GLFW_INIT;
		return;
	}

	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif


	// Create window with graphics context
	window = glfwCreateWindow(1280, 720, "Physics Sim", NULL, NULL);
	if (window == NULL) {
		error = errorType::GLFW_WINDOW;
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR WITH GLEW" << std::endl;
		error = errorType::GLEW_INIT;
		return;
	}

	glEnable(GL_TEXTURE_2D);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}