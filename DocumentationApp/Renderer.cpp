#include "Renderer.h"


Renderer::Renderer(GLFWwindow* win, Simulation* sim) : window(win), simulation(sim){

}

void Renderer::newFrame() {
	// New ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport();

	// Update window size
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);

	// Clear opengl window buffer
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::renderImGUI() {
	ImGuiIO& io = ImGui::GetIO(); (void)io;


	if (showDataWindow) {
		ImGui::Begin("Data Window");

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::End();
	}

	// RENDER MORE GUI HERE

	// Rendering
	ImGui::Render();

	// Swap buffers to OpenGL
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Allows for ImGUI windows to be outside the main window
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void Renderer::renderSimulation() {

}

void Renderer::renderCircle(glm::vec2 position, double radius, ImVec4 colour) {

}

void Renderer::renderPolygon(glm::vec2 position, std::vector<glm::vec2> verticies, ImVec4 colour) {

}

ImVec2 Renderer::getWindowSize() {
	return wsize;
}