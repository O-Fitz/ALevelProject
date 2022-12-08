#include "Renderer.h"

Renderer::Renderer(GLFWwindow* win, Simulation* sim) : window(win), simulation(sim) {
	
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

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&background); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	// Rendering
	ImGui::Render();
	
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
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
	glClearColor(background.x * background.w, background.y * background.w, background.z * background.w, background.w);
	glClear(GL_COLOR_BUFFER_BIT);

	// RENDER SIMULATION HERE

	std::vector<PBody> n = simulation->getBodies();
	for (int i = 0; i < n.size(); i++) {
		n[i]->render(this);
	}

	renderCircle(glm::vec2(wsize.x/2, wsize.y/2), 10, ImVec4(1.0, 0.0, 0.0, 1.0));

	// Unbinds Frame buffer
	fb.unbind();

	// Draws the texture linked to the frame buffer to the screen
	ImGui::GetWindowDrawList()->AddImage(
		(void*)fb.get_texture(), ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2(pos.x + wsize.x, pos.y + wsize.y), ImVec2(0, 1), ImVec2(1, 0));

	// End of ImGui window
	ImGui::End();
	ImGui::PopStyleVar(ImGuiStyleVar_WindowPadding);


	// TEST TEXTURE ON BACKGROUND
	/*glBindTexture(GL_TEXTURE_2D, fb.get_texture());
	glBegin(GL_TRIANGLES);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	glVertex2f(-1, -1);
	glVertex2f(-1, 1);
	glVertex2f(1, 1);

	glVertex2f(1, 1);
	glVertex2f(1, -1);
	glVertex2f(-1, -1);

	glEnd();
	renderCircle(glm::vec2(1, 1), 0.5);*/


}

void Renderer::renderCircle(glm::vec2 position, double radius, ImVec4 colour) {
	glBegin(GL_TRIANGLE_FAN);
	// Change colour
	glColor4f(colour.x, colour.y, colour.z, colour.w);
	glVertex2f(position.x, position.y);
	
	for (double i = 0; i <= 2 * PI + 0.1; i += 0.1)
	{
		// Rotates 0.1 rads round the circle and draws a line
		glVertex2f(position.x + sin(i) * radius, position.y + cos(i) * radius);
	}
	glEnd();
}

void Renderer::renderPolygon(glm::vec2 position, std::vector<glm::vec2> verticies, ImVec4 colour) {

	glBegin(GL_TRIANGLE_FAN);
	glColor4f(colour.x, colour.y, colour.z, colour.w);

	for (const glm::vec2& vertex : verticies) {
		glVertex2d(pos.x+vertex.x, pos.y+vertex.y);
	}

	glEnd();
}

ImVec2 Renderer::getWindowSize() {
	return wsize;
}
