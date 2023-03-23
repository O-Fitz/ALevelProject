#include "Renderer.h"
#include "Body.h"


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

	// Setup Projection matrix
	projection = glm::ortho(0.0f, wsize.x, 0.0f, wsize.y, -1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION_MATRIX);
	glLoadMatrixf(glm::value_ptr(projection));

	// Setup ImGui window with no border
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Viewport");

	// Updates the window position and size
	wsize = ImGui::GetWindowSize();
	pos = ImGui::GetCursorScreenPos();

	// Initialises and binds Frame Buffer
	fb.create_buffers(wsize.x, wsize.y);
	fb.bind();

	// Clear the window
	glClearColor(background.x, background.y, background.z, background.w);
	glClear(GL_COLOR_BUFFER_BIT);

	// Renders Simulation
	std::vector<PBody> n = simulation->getBodies();
	for (int i = 0; i < n.size(); i++) {
		n[i]->render(this);
	}

	// Unbinds Frame buffer
	fb.unbind();

	// Draws the texture linked to the frame buffer to the screen
	ImGui::GetWindowDrawList()->AddImage(
		(void*)fb.get_texture(), ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2(pos.x + wsize.x, pos.y + wsize.y), ImVec2(0, 1), ImVec2(1, 0));

	// End of ImGui window
	ImGui::End();
	ImGui::PopStyleVar(ImGuiStyleVar_WindowPadding);

}

void Renderer::drawCircle(glm::vec2 position, double radius, ImVec4 colour) {
	glBegin(GL_TRIANGLE_FAN);
	// Change colour
	glColor4f(colour.x, colour.y, colour.z, colour.w);
	glVertex2f(position.x, position.y);

	for (double i = 0; i <= 2 * 3.141 + 0.1; i += 0.1)
	{
		// Rotates 0.1 rads round the circle and draws a triangle
		glVertex2f(position.x + sin(i) * radius, position.y + cos(i) * radius);
	}
	glEnd();
}

void Renderer::drawPolygon(glm::vec2 position, std::vector<glm::vec2> vertices, ImVec4 colour) {
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(colour.x, colour.y, colour.z, colour.w);

	for (const glm::vec2& vertex : vertices) {
		glVertex2d(position.x + vertex.x, position.y + vertex.y);
	}

	glEnd();
}

void Renderer::drawLine(glm::vec2 pos1, glm::vec2 pos2, ImVec4 colour) {
	glBegin(GL_LINES);
	glColor4f(colour.x, colour.y, colour.z, colour.w);
	glVertex2f(pos1.x, pos1.y);
	glVertex2f(pos2.x, pos2.y);
	glEnd();
}

ImVec2 Renderer::getWindowSize() {
	return wsize;
}